<?php
// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2013 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

// This file contains a PHP binding of a web-service interface
// for submitting jobs to a BOINC server.
//
// Functions:
//      boinc_abort_batch(): abort a batch
//      boinc_estimate_batch(); estimate completion time of a batch
//      boinc_get_output_file(): get the URL for an output file
//      boinc_get_output_files(): get the URL for zipped batch output
//      boinc_query_batch(): get details of a batch
//      boinc_query_batches(): get list of batches
//      boinc_query_job(): get details of a job
//      boinc_retire_batch(): retire a batch; delete output files
//      boinc_submit_batch(): submit a batch

//// Implementation stuff follows

function req_to_xml($req, $op) {
    $x = "<$op>
    <authenticator>$req->authenticator</authenticator>
    <batch>
    <app_name>$req->app_name</app_name>
    <batch_name>$req->batch_name</batch_name>
";
    foreach ($req->jobs as $job) {
        $x .= "    <job>
        <rsc_fpops_est>$job->rsc_fpops_est</rsc_fpops_est>
        <command_line>$job->command_line</command_line>
";
        foreach ($job->input_files as $file) {
            $x .= "        <input_file>\n";
            $x .= "            <mode>$file->mode</mode>\n";
            $x .= "            <source>$file->source</source>\n";
            $x .= "        </input_file>\n";
        }
        $x .= "    </job>
";
    }
    $x .= "    </batch>
</$op>
";
    return $x;
}

function validate_request($req) {
    if (!is_object($req)) return "req is not an object";
    if (!array_key_exists('project', $req)) return "missing req->project";
    if (!array_key_exists('authenticator', $req)) return "missing req->authenticator";
    if (!array_key_exists('app_name', $req)) return "missing req->app_name";
    if (!array_key_exists('jobs', $req)) return "missing req->jobs";
    if (!is_array($req->jobs)) return "req->jobs is not an array";
    foreach ($req->jobs as $job) {
        // other checks
    }
    return null;
}

function do_http_op($req, $xml, $op) {
    $ch = curl_init("$req->project/submit_rpc_handler.php");
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    // see if we need to send any files
    //
    $nfiles = 0;
    $post = array();
    $post["request"] = $xml;
    $cwd = getcwd();
    if ($op == "submit_batch") {
        foreach ($req->jobs as $job) {
            foreach ($job->input_files as $file) {
                if ($file->mode == "inline") {
                    $path = realpath("$cwd/$file->path");
                    $post["file$nfiles"] = $path;
                    $nfiles++;
                }
            }
        }
    }
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
    $reply = curl_exec($ch);
    if (!$reply) return array(null, "HTTP error");
    $r = simplexml_load_string($reply);
    if (!$r) return array(null, "Can't parse reply XML: <pre>".htmlentities($reply)."</pre>");
    $e = (string)$r->error_msg;
    if ($e) {
        return array(null, $e);
    } else {
        return array($r, null);
    }
}

function do_batch_op($req, $op) {
    $retval = validate_request($req);
    if ($retval) return array(null, $retval);
    $xml = req_to_xml($req, $op);
    return do_http_op($req, $xml, $op);
}

function batch_xml_to_object($batch) {
    $b = null;
    $b->id = (int)($batch->id);
    $b->create_time = (double)($batch->create_time);
    $b->est_completion_time = (double)($batch->est_completion_time);
    $b->njobs = (int)($batch->njobs);
    $b->fraction_done = (double) $batch->fraction_done;
    $b->nerror_jobs = (int)($batch->nerror_jobs);
    $b->state = (int)($batch->state);
    $b->completion_time = (double)($batch->completion_time);
    $b->credit_estimate = (double)($batch->credit_estimate);
    $b->credit_canonical = (double)($batch->credit_canonical);
    $b->name = (string)($batch->name);
    $b->app_name = (string)($batch->app_name);
    return $b;
}

//// API functions follow

function boinc_estimate_batch($req) {
    list($reply, $errmsg) = do_batch_op($req, "estimate_batch");
    if ($errmsg) return array(0, $errmsg);
    $name = $reply->getName();
    if ($name == 'estimate') {
        return array((string)$reply->seconds, null);
    } else if ($name = 'error') {
        return array(null, (string)$reply->message);
    } else {
        return array(null, "Bad reply message");
    }
}

function boinc_submit_batch($req) {
    list($reply, $errmsg) = do_batch_op($req, "submit_batch");
    if ($errmsg) return array(0, $errmsg);
    $name = $reply->getName();
    if ($name == 'batch_id') {
        return array((int)$reply, null);
    }  else if ($name == 'error') {
        return array(null, (string)$reply->err_message);
    } else {
        return array(null, "Bad reply message");
    }
}

function boinc_query_batches($req) {
    $req_xml = "<query_batches>
    <authenticator>$req->authenticator</authenticator>
</query_batches>
";
    list($reply, $errmsg) = do_http_op($req, $req_xml, "");
    if ($errmsg) return array(null, $errmsg);
    $batches = array();
    foreach ($reply->batch as $batch) {
        $b = batch_xml_to_object($batch);
        $batches[] = $b;
    }
    return array($batches, null);
}

function boinc_query_batch($req) {
    $req_xml = "<query_batch>
    <authenticator>$req->authenticator</authenticator>
    <batch_id>$req->batch_id</batch_id>
</query_batch>
";
    list($reply, $errmsg) = do_http_op($req, $req_xml, "");
    if ($errmsg) return array(null, $errmsg);
    $jobs = array();
    foreach ($reply->job as $job) {
        $j = null;
        $j->id = (int)($job->id);
        $j->canonical_instance_id = (int)($job->canonical_instance_id);
        $jobs[] = $j;
    }
    $r = batch_xml_to_object($reply);
    $r->jobs = $jobs;
    return array($r, null);
}

function boinc_query_job($req) {
    $req_xml = "<query_job>
    <authenticator>$req->authenticator</authenticator>
    <job_id>$req->job_id</job_id>
</query_job>
";
    list($reply, $errmsg) = do_http_op($req, $req_xml, "");
    if ($errmsg) return array(null, $errmsg);
    $instances = array();
    foreach ($reply->instance as $instance) {
        $i = null;
        $i->name = (string)($instance->name);
        $i->id = (int)($instance->id);
        $i->state = (string)($instance->state);
        $i->outfiles = array();
        foreach ($instance->outfile as $outfile) {
            $f = null;
            $f->size = (double)$outfile->size;
            $i->outfiles[] = $f;
        }
        $instances[] = $i;
    }
    $r = null;
    $r->instances = $instances;
    return array($r, null);
}

function boinc_abort_batch($req) {
    $req_xml = "<abort_batch>
    <authenticator>$req->authenticator</authenticator>
    <batch_id>$req->batch_id</batch_id>
</abort_batch>
";
    list($reply, $errmsg) = do_http_op($req, $req_xml, "");
    if ($errmsg) return $errmsg;
    $name = $reply->getName();
    if ($name == 'error') {
        return (string)($reply->message);
    }
    return null;
}

function boinc_get_output_file($req) {
    $auth_str = md5($req->authenticator.$req->instance_name);
    $name = $req->instance_name;
    $file_num = $req->file_num;
    return $req->project."/get_output.php?cmd=result_file&result_name=$name&file_num=$file_num&auth_str=$auth_str";
}

function boinc_get_output_files($req) {
    $auth_str = md5($req->authenticator.$req->batch_id);
    $batch_id = $req->batch_id;
    return $req->project."/get_output.php?cmd=batch_files&batch_id=$batch_id&auth_str=$auth_str";
}

function boinc_retire_batch($req) {
    $req_xml = "<retire_batch>
    <authenticator>$req->authenticator</authenticator>
    <batch_id>$req->batch_id</batch_id>
</retire_batch>
";
    list($reply, $errmsg) = do_http_op($req, $req_xml, "");
    if ($errmsg) return $errmsg;
    $name = $reply->getName();
    if ($name == 'error') {
        return (string)($reply->message);
    }
    return null;
}

//// example usage follows

$req->project = "http://isaac.ssl.berkeley.edu/test/";
$req->authenticator = "x";
if (0) {
    $req->app_name = "uppercase";
    $req->batch_name = "batch_name3";
    $req->jobs = array();

    $job->input_files = array();
    $f->source = "http://isaac.ssl.berkeley.edu/validate_logic.txt";
    $f->mode = "remote";
    $f->md5 = "eec5a142cea5202c9ab2e4575a8aaaa7";
    $f->size = 4250;
    $job->input_files[] = $f;
    $f = null;
    $f->mode = "local";
    $f->source = "foobar";
    //$job->input_files[] = $f;

    for ($i=10; $i<11; $i++) {
        $job->rsc_fpops_est = $i*1e9;
        $job->command_line = "--t $i";
        $req->jobs[] = $job;
    }

    if (0) {
        list($e, $errmsg) = boinc_estimate_batch($req);
        if ($errmsg) {
            echo "Error from server: $errmsg\n";
        } else {
            echo "Batch completion estimate: $e seconds\n";
        }
    } else {
        list($id, $errmsg) = boinc_submit_batch($req);
        if ($errmsg) {
            echo "Error from server: $errmsg\n";
        } else {
            echo "Batch ID: $id\n";
        }
    }
}

if (0) {
    list($batches, $errmsg) = boinc_query_batches($req);
    if ($errmsg) {
        echo "Error: $errmsg\n";
    } else {
        print_r($batches);
    }
}

if (0) {
    $req->batch_id = 20;
    list($jobs, $errmsg) = boinc_query_batch($req);
    if ($errmsg) {
        echo "Error: $errmsg\n";
    } else {
        print_r($jobs);
    }
}

?>

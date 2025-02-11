// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2008 University of California
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
//
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "WelcomePage.h"
#endif

#include "stdwx.h"
#include "diagnostics.h"
#include "util.h"
#include "mfile.h"
#include "miofile.h"
#include "parse.h"
#include "error_numbers.h"
#include "wizardex.h"
#include "error_numbers.h"
#include "BOINCGUIApp.h"
#include "SkinManager.h"
#include "MainDocument.h"
#include "BOINCBaseWizard.h"
#include "WizardAttach.h"
#include "WelcomePage.h"

////@begin XPM images
////@end XPM images


/*!
 * CWelcomePage type definition
 */
 
IMPLEMENT_DYNAMIC_CLASS( CWelcomePage, wxWizardPageEx )
 
/*!
 * CWelcomePage event table definition
 */
 
BEGIN_EVENT_TABLE( CWelcomePage, wxWizardPageEx )
 
////@begin CWelcomePage event table entries
    EVT_WIZARDEX_PAGE_CHANGED( -1, CWelcomePage::OnPageChanged )
    EVT_RADIOBUTTON( ID_WELCOMESELECTWIZARDPROJECT, CWelcomePage::OnWizardSelectionChanged )
    EVT_RADIOBUTTON( ID_WELCOMESELECTWIZARDACCOUNTMGR, CWelcomePage::OnWizardSelectionChanged )
    EVT_RADIOBUTTON( ID_WELCOMECHANGEAPPS, CWelcomePage::OnWizardSelectionChanged )
    EVT_WIZARDEX_CANCEL( -1, CWelcomePage::OnCancel )
////@end CWelcomePage event table entries
 
END_EVENT_TABLE()

/*!
 * CWelcomePage constructors
 */
 
CWelcomePage::CWelcomePage( )
{
}
 
CWelcomePage::CWelcomePage( CBOINCBaseWizard* parent )
{
    Create( parent );
}
 
/*!
 * WizardPage creator
 */
 
bool CWelcomePage::Create( CBOINCBaseWizard* parent )
{
////@begin CWelcomePage member initialisation
    m_pTitleStaticCtrl = NULL;
    m_pDescriptionStaticCtrl = NULL;
    m_pAttachToProjectCtrl = NULL;
    m_pAttachToAccountManagerCtrl = NULL;
    m_pDirectionsStaticCtrl = NULL;
////@end CWelcomePage member initialisation

#if defined (_WCG)
	((CWizardAttach*)parent)->IsFirstPass = true;
#else
	((CWizardAttach*)parent)->IsFirstPass = false;
#endif
 
////@begin CWelcomePage creation
    wxWizardPageEx::Create( parent, ID_WELCOMEPAGE );

    CreateControls();
    GetSizer()->Fit(this);
////@end CWelcomePage creation

	return TRUE;
}
 
/*!
 * Control creation for WizardPage
 */
 
void CWelcomePage::CreateControls()
{    
////@begin CWelcomePage content construction
    CWelcomePage* itemWizardPage2 = this;

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemWizardPage2->SetSizer(itemBoxSizer3);

    m_pTitleStaticCtrl = new wxStaticText;
    m_pTitleStaticCtrl->Create( itemWizardPage2, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_pTitleStaticCtrl->SetFont(wxFont(12, wxSWISS, wxNORMAL, wxBOLD, FALSE, _T("Verdana")));
    itemBoxSizer3->Add(m_pTitleStaticCtrl, 0, wxALIGN_LEFT|wxALL, 5);

    m_pDescriptionStaticCtrl = new wxStaticText;
    m_pDescriptionStaticCtrl->Create( itemWizardPage2, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(m_pDescriptionStaticCtrl, 0, wxALIGN_LEFT|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer62 = new wxFlexGridSizer(2, 1, 0, 0);
    itemFlexGridSizer62->AddGrowableCol(1);
    itemBoxSizer3->Add(itemFlexGridSizer62, 0, wxGROW|wxALL, 5);

    m_pChangeApplicationsCtrl = new wxRadioButton;
    m_pChangeApplicationsCtrl->Create(itemWizardPage2, ID_WELCOMECHANGEAPPS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
#if defined (_WCG)
	m_pChangeApplicationsCtrl->SetValue(TRUE);
#else
	m_pChangeApplicationsCtrl->SetValue(FALSE);
#endif
    itemFlexGridSizer62->Add(m_pChangeApplicationsCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_pAttachToProjectCtrl = new wxRadioButton;
    m_pAttachToProjectCtrl->Create( itemWizardPage2, ID_WELCOMESELECTWIZARDPROJECT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
#if defined (_WCG)
    m_pAttachToProjectCtrl->SetValue(FALSE);
#else
    m_pAttachToProjectCtrl->SetValue(TRUE);
#endif
    itemFlexGridSizer62->Add(m_pAttachToProjectCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_pAttachToAccountManagerCtrl = new wxRadioButton;
    m_pAttachToAccountManagerCtrl->Create( itemWizardPage2, ID_WELCOMESELECTWIZARDACCOUNTMGR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_pAttachToAccountManagerCtrl->SetValue(FALSE);
    itemFlexGridSizer62->Add(m_pAttachToAccountManagerCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_pDirectionsStaticCtrl = new wxStaticText;
    m_pDirectionsStaticCtrl->Create( itemWizardPage2, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(m_pDirectionsStaticCtrl, 0, wxALIGN_LEFT|wxALL, 5);

    itemWizardPage2->SetSizer(itemBoxSizer3);

#ifdef __WXMAC__
    //Accessibility
    HIViewRef buttonView = (HIViewRef)m_pAttachToProjectCtrl->GetHandle();
    HIObjectRef   theObject = (HIObjectRef)HIViewGetSuperview(buttonView);
    HIObjectSetAccessibilityIgnored(theObject, true);
#endif
////@end CWelcomePage content construction
}

/*
 * Gets the previous page.
 */
 
wxWizardPageEx* CWelcomePage::GetPrev() const
{
    return NULL;
}
 
/*!
 * Gets the next page.
 */
 
wxWizardPageEx* CWelcomePage::GetNext() const
{
    if (CHECK_CLOSINGINPROGRESS()) {
        // Cancel Event Detected
        return PAGE_TRANSITION_NEXT(ID_COMPLETIONERRORPAGE);
    } else if (IS_ATTACHTOPROJECTWIZARD()) {
        return PAGE_TRANSITION_NEXT(ID_PROJECTINFOPAGE);
    } else if (IS_ACCOUNTMANAGERUPDATEWIZARD()) {
        return PAGE_TRANSITION_NEXT(ID_ACCOUNTMANAGERPROCESSINGPAGE);
    } else if (IS_ACCOUNTMANAGERWIZARD()) {
        return PAGE_TRANSITION_NEXT(ID_ACCOUNTMANAGERINFOPAGE);
	} else if (IS_CHANGEWCGAPPS()) {
		if ( ((CWizardAttach*)GetParent())->IsFirstPass ) {
			((CWizardAttach*)GetParent())->IsFirstPass = false;
		} else {
			this->OpenWorldCommunityGridProjectsPage();
		}
	}
    return NULL;
}
 
/*!
 * Should we show tooltips?
 */
 
bool CWelcomePage::ShowToolTips()
{
    return TRUE;
}
 
/*!
 * Get bitmap resources
 */
 
wxBitmap CWelcomePage::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin CWelcomePage bitmap retrieval
    return wxNullBitmap;
////@end CWelcomePage bitmap retrieval
}
 
/*!
 * Get icon resources
 */
 
wxIcon CWelcomePage::GetIconResource( const wxString& WXUNUSED(name) )
{
    // Icon retrieval
////@begin CWelcomePage icon retrieval
    return wxNullIcon;
////@end CWelcomePage icon retrieval
}
   
/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WELCOMEPAGE
 */
 
void CWelcomePage::OnPageChanged( wxWizardExEvent& event ) {
    if (event.GetDirection() == false) return;
    wxLogTrace(wxT("Function Start/End"), wxT("CWelcomePage::OnPageChanged - Function Begin"));

    CMainDocument*         pDoc = wxGetApp().GetDocument();
    ACCT_MGR_INFO          ami;
    bool                   is_acct_mgr_detected = false;
    bool                   is_wcg_client = false;
    wxString               strBuffer = wxEmptyString;


    wxASSERT(pDoc);
    wxASSERT(wxDynamicCast(pDoc, CMainDocument));
    wxASSERT(m_pTitleStaticCtrl);
    wxASSERT(m_pDescriptionStaticCtrl);
    wxASSERT(m_pAttachToProjectCtrl);
    wxASSERT(m_pAttachToAccountManagerCtrl);
    wxASSERT(m_pDirectionsStaticCtrl);
    wxASSERT(m_pChangeApplicationsCtrl);


#if defined (_WCG)
    // Determine if we are the World Community Grid version of the client
    // and connected to World Community Grid
	std::string wcgUrl = "http://www.worldcommunitygrid.org/";
	if ( pDoc->state.lookup_project(wcgUrl.c_str()) ) {
        is_wcg_client = true;
    }
#endif


	if ( !is_wcg_client ) {
		m_pTitleStaticCtrl->SetLabel(
			_("Add project or account manager")
		);
	} else {
		m_pTitleStaticCtrl->SetLabel(
			_("Add project or use BOINC Account Manager")
		);
	}

    pDoc->rpc.acct_mgr_info(ami);
    is_acct_mgr_detected = ami.acct_mgr_url.size() ? true : false;

    if (is_acct_mgr_detected) {
        strBuffer.Printf(
            _("If possible, add projects at the\n%s web site.\n\nProjects added via this wizard will not be\nlisted on or managed via %s."), 
            wxString(ami.acct_mgr_name.c_str(), wxConvUTF8).c_str(),
            wxString(ami.acct_mgr_name.c_str(), wxConvUTF8).c_str()
        );

        m_pDescriptionStaticCtrl->SetLabel(
            strBuffer
        );

		m_pAttachToAccountManagerCtrl->SetLabel(
			_("Use account manager")
		);
        m_pAttachToAccountManagerCtrl->Disable();
    } else {
        if (!is_wcg_client) {
            m_pDescriptionStaticCtrl->SetLabel(
_("There are over 30 BOINC-based projects\n\
doing research in many areas of science,\n\
and you can volunteer for as many of them as you like.\n\
You can add a project directly,\n\
or use an 'Account Manager' web site to select projects.")
            );

			m_pAttachToAccountManagerCtrl->SetLabel(
				_("Use account manager")
			);
        } else {
            m_pDescriptionStaticCtrl->SetLabel(
_("You have chosen to add a new volunteer computing project or change which projects\n\
you contribute to.\n\
\n\
Some of these projects are run and managed by World Community Grid, while others\n\
are run and managed by other researchers or organizations. The BOINC software\n\
can divide your spare processing power among any combination of projects.\n\
\n\
Alternatively, if you have registered with a BOINC Account Manager, you can use\n\
this to choose which projects to support.\n\
\n\
Please choose which type of change you would like to make:\n")
            );

			m_pAttachToAccountManagerCtrl->SetLabel(
				_("Use a BOINC Account Manager")
			);
        }

		m_pAttachToAccountManagerCtrl->Enable();
    }

    if (!is_wcg_client) {
		m_pAttachToProjectCtrl->SetLabel(
			_("Add project")
		);

        m_pDirectionsStaticCtrl->SetLabel(
            _("To continue, click Next.")
        );

        m_pChangeApplicationsCtrl->Hide();
	} else {
        m_pChangeApplicationsCtrl->SetLabel(
			 _("Add or change your World Community Grid projects")
        );
		m_pAttachToProjectCtrl->SetLabel(
			_("Add projects run by other researchers or organizations")
		);
	}
 
    Fit();
    wxLogTrace(wxT("Function Start/End"), wxT("CWelcomePage::OnPageChanged - Function End"));
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_WELCOMESELECTWIZARDPROJECT
 *   or ID_WELCOMESELECTWIZARDACCOUNTMGR
 */
 
void CWelcomePage::OnWizardSelectionChanged( wxCommandEvent& event ) {
    wxLogTrace(wxT("Function Start/End"), wxT("CWelcomePage::OnWizardSelectionChanged - Function Begin"));

    CWizardAttach*  pWAP = ((CWizardAttach*)GetParent());

    wxASSERT(pWAP);
    wxASSERT(wxDynamicCast(pWAP, CWizardAttach));

    if (ID_WELCOMESELECTWIZARDPROJECT == event.GetId()) {
        pWAP->IsAttachToProjectWizard = true;
        pWAP->IsAccountManagerWizard = false;
		pWAP->IsChangeWCGApps = false;
    } else if (ID_WELCOMESELECTWIZARDACCOUNTMGR == event.GetId()) {
        pWAP->IsAttachToProjectWizard = false;
        pWAP->IsAccountManagerWizard = true;
		pWAP->IsChangeWCGApps = false;
	} else if (ID_WELCOMECHANGEAPPS == event.GetId()) {
        pWAP->IsAttachToProjectWizard = false;
        pWAP->IsAccountManagerWizard = false;
		pWAP->IsChangeWCGApps = true;
	}

    wxLogTrace(wxT("Function Start/End"), wxT("CWelcomePage::OnWizardSelectionChanged - Function End"));
}


/*!
 * wxEVT_BUTTON event handler for ID_WELCOMECHANGEAPPS
 */

void CWelcomePage::OpenWorldCommunityGridProjectsPage() const {
    wxLogTrace(wxT("Function Start/End"), wxT("CWelcomePage::OpenWorldCommunityGridProjectsPage - Function Begin"));
    CWizardAttach*  pWAP = ((CWizardAttach*)GetParent());

    wxASSERT(pWAP);
    wxASSERT(wxDynamicCast(pWAP, CWizardAttach));

    wxLaunchDefaultBrowser(wxT("http://www.worldcommunitygrid.org/ms/viewMyProjects.do"));

    pWAP->SimulateCancelButton();

    wxLogTrace(wxT("Function Start/End"), wxT("CWelcomePage::OpenWorldCommunityGridProjectsPage - Function End"));
}


/*!
 * wxEVT_WIZARD_CANCEL event handler for ID_WELCOMEPAGE
 */
 
void CWelcomePage::OnCancel( wxWizardExEvent& event ) {
    PROCESS_CANCELEVENT(event);
}

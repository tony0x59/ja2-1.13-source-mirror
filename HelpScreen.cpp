#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "HelpScreen.h"
	#include "HelpScreenText.h"
	#include	"Line.h"
#else
	#include "sgp.h"
	#include "sysutil.h"
	#include "vobject_blitters.h"
	#include "wcheck.h"
	#include "cursors.h"
	#include "font control.h"
	#include "cursor control.h"
	#include "render dirty.h"
	#include	"Text.h"
	#include	"Utilities.h"
	#include	"WordWrap.h"
	#include "Font Control.h"
	#include "text.h"
	#include "HelpScreen.h"
	#include "HelpScreenText.h"
	#include "Line.h"
	#include "Game Clock.h"
	#include "GameSettings.h"
	#include "laptop.h"
	#include "Text Input.h"
	#include "english.h"
	#include "renderworld.h"
	#include "Game Init.h"
	#include "Overhead.h"
#endif

extern INT16 gsVIEWPORT_END_Y;
extern void PrintDate( void );
extern void PrintNumberOnTeam( void );
extern void PrintBalance( void );
extern		BOOLEAN fMapScreenBottomDirty;
extern		BOOLEAN fCharacterInfoPanelDirty;
extern		BOOLEAN fTeamPanelDirty;
extern		BOOLEAN fMapScreenBottomDirty;
extern		BOOLEAN fMapPanelDirty;
extern		BOOLEAN	gfGamePaused;
extern		BOOLEAN fShowMapInventoryPool;

extern		BOOLEAN BltVSurfaceUsingDD( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, UINT32 fBltFlags, INT32 iDestX, INT32 iDestY, RECT *SrcRect );



#define			HELP_SCREEN_ACTIVE			0x00000001



//The defualt size and placement of the screen
//#define			HELP_SCREEN_DEFUALT_LOC_X							155
//#define			HELP_SCREEN_DEFUALT_LOC_Y							105

#define			HELP_SCREEN_DEFUALT_LOC_WIDTH					HELP_SCREEN_SMALL_LOC_WIDTH + HELP_SCREEN_BUTTON_BORDER_WIDTH
#define			HELP_SCREEN_DEFUALT_LOC_HEIGHT				292//300

#define			HELP_SCREEN_BUTTON_BORDER_WIDTH				92
#define			HELP_SCREEN_SMALL_LOC_WIDTH						320
#define			HELP_SCREEN_SMALL_LOC_HEIGHT					HELP_SCREEN_DEFUALT_LOC_HEIGHT //224



#define			HELP_SCREEN_BTN_OFFSET_X							11
#define			HELP_SCREEN_BTN_OFFSET_Y							12//50
#define			HELP_SCREEN_BTN_FONT_ON_COLOR					73
#define			HELP_SCREEN_BTN_FONT_OFF_COLOR				FONT_MCOLOR_WHITE

#define			HELP_SCREEN_BTN_FONT_BACK_COLOR				50
#define			HELP_SCREEN_BTN_FONT									FONT10ARIAL

#define			HELP_SCREEN_BTN_WIDTH									77
#define			HELP_SCREEN_BTN_HEIGHT								22
#define			HELP_SCREEN_GAP_BN_BTNS								8

#define			HELP_SCREEN_MARGIN_SIZE								10
#define			HELP_SCREEN_TEXT_RIGHT_MARGIN_SPACE		36
#define			HELP_SCREEN_TEXT_LEFT_MARGIN_WITH_BTN	( HELP_SCREEN_BUTTON_BORDER_WIDTH + 5 + HELP_SCREEN_MARGIN_SIZE )
#define			HELP_SCREEN_TEXT_LEFT_MARGIN					( 5 + HELP_SCREEN_MARGIN_SIZE )


#define			HELP_SCREEN_TEXT_OFFSET_Y							48
#define			HELP_SCREEN_GAP_BTN_LINES							2

#define			HELP_SCREEN_TITLE_BODY_FONT						FONT12ARIAL
#define			HELP_SCREEN_TITLE_BODY_COLOR					FONT_MCOLOR_WHITE//FONT_NEARBLACK

#define			HELP_SCREEN_TEXT_BODY_FONT						FONT10ARIAL
#define			HELP_SCREEN_TEXT_BODY_COLOR						FONT_MCOLOR_WHITE//FONT_NEARBLACK
#define			HELP_SCREEN_TEXT_BACKGROUND						0//NO_SHADOW//FONT_MCOLOR_WHITE

#define			HELP_SCREEN_TITLE_OFFSET_Y						7
#define			HELP_SCREEN_HELP_REMINDER_Y						HELP_SCREEN_TITLE_OFFSET_Y + 15

#define			HELP_SCREEN_NUM_BTNS									8


#define			HELP_SCREEN_SHOW_HELP_AGAIN_REGION_OFFSET_X		4
#define			HELP_SCREEN_SHOW_HELP_AGAIN_REGION_OFFSET_Y		18
#define			HELP_SCREEN_SHOW_HELP_AGAIN_REGION_TEXT_OFFSET_X	25 + HELP_SCREEN_SHOW_HELP_AGAIN_REGION_OFFSET_X
#define			HELP_SCREEN_SHOW_HELP_AGAIN_REGION_TEXT_OFFSET_Y	( HELP_SCREEN_SHOW_HELP_AGAIN_REGION_OFFSET_Y )

#define			HELP_SCREEN_EXIT_BTN_OFFSET_X									291
#define			HELP_SCREEN_EXIT_BTN_LOC_Y										9

#define			HELP_SCREEN_


//the type of help screen
#define HLP_SCRN_DEFAULT_TYPE		9
#define HLP_SCRN_BUTTON_BORDER	8



//this is the size of the text buffer where everything will be blitted.
// 2 ( bytest for char ) * width of buffer * height of 1 line * # of text lines
//#define	HLP_SCRN__NUMBER_BYTES_IN_TEXT_BUFFER						( 2 * HLP_SCRN__WIDTH_OF_TEXT_BUFFER * HLP_SCRN__HEIGHT_OF_1_LINE_IN_BUFFER * HLP_SCRN__MAX_NUMBER_OF_LINES_IN_BUFFER )
#define	HLP_SCRN__WIDTH_OF_TEXT_BUFFER									280
#define	HLP_SCRN__MAX_NUMBER_OF_LINES_IN_BUFFER					170//100
#define	HLP_SCRN__HEIGHT_OF_1_LINE_IN_BUFFER						( GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT ) + HELP_SCREEN_GAP_BTN_LINES )
#define	HLP_SCRN__MAX_NUMBER_PIXELS_DISPLAYED_IN_TEXT_BUFFER		HELP_SCREEN_DEFUALT_LOC_HEIGHT
#define	HLP_SCRN__HEIGHT_OF_TEXT_BUFFER									( HLP_SCRN__HEIGHT_OF_1_LINE_IN_BUFFER * HLP_SCRN__MAX_NUMBER_OF_LINES_IN_BUFFER )

#define	HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER		( HLP_SCRN__HEIGHT_OF_TEXT_AREA / HLP_SCRN__HEIGHT_OF_1_LINE_IN_BUFFER )

#define	HLP_SCRN__HEIGHT_OF_TEXT_AREA											228

#define	HLP_SCRN__HEIGHT_OF_SCROLL_AREA									182
#define HLP_SCRN__WIDTH_OF_SCROLL_AREA									20
#define HLP_SCRN__SCROLL_POSX														292
#define HLP_SCRN__SCROLL_POSY														( gHelpScreen.usScreenLocY + 63 )

#define	HLP_SCRN__SCROLL_UP_ARROW_X											292
#define	HLP_SCRN__SCROLL_UP_ARROW_Y											43

#define	HLP_SCRN__SCROLL_DWN_ARROW_X										HLP_SCRN__SCROLL_UP_ARROW_X
#define	HLP_SCRN__SCROLL_DWN_ARROW_Y										HLP_SCRN__SCROLL_UP_ARROW_Y + 202




//enums for the different dirty levels
enum
{
	HLP_SCRN_DRTY_LVL_NOT_DIRTY,
	HLP_SCRN_DRTY_LVL_REFRESH_TEXT,
	HLP_SCRN_DRTY_LVL_REFRESH_ALL,
};

//new screen:

enum
{
	HLP_SCRN_MPSCRN_SCTR_OVERVIEW,
};

//mapscreen, welcome to arulco
enum
{
	HLP_SCRN_MPSCRN_OVERVIEW,
	HLP_SCRN_MPSCRN_ASSIGNMENTS,
	HLP_SCRN_MPSCRN_DESTINATIONS,
	HLP_SCRN_MPSCRN_MAP,
	HLP_SCRN_MPSCRN_MILITIA,
	HLP_SCRN_MPSCRN_AIRSPACE,
	HLP_SCRN_MPSCRN_ITEMS,
	HLP_SCRN_MPSCRN_KEYBOARD,

	HLP_SCRN_NUM_MPSCRN_BTNS,

};
//laptop sub pages
enum
{
	HLP_SCRN_LPTP_OVERVIEW,
	HLP_SCRN_LPTP_EMAIL,
	HLP_SCRN_LPTP_WEB,
	HLP_SCRN_LPTP_FILES,
	HLP_SCRN_LPTP_HISTORY,
	HLP_SCRN_LPTP_PERSONNEL,
	HLP_SCRN_LPTP_FINANCIAL,
	HLP_SCRN_LPTP_MERC_STATS,

	HLP_SCRN_LPTP_NUM_PAGES,
};

//Mapscreen no one hired yet pages
enum
{
	HLP_SCRN_NO_ONE_HIRED,

	HLP_SCRN_NUM_MAPSCREEN_NO_1_HIRED_YET_PAGES,
};

//mapscreen no 1 hired yet pages
enum
{
	HLP_SCRN_NOT_IN_ARULCO,

	HLP_SCRN_NUM_NOT_IN_ARULCO_PAGES,
};



//Tactical
enum
{
	HLP_SCRN_TACTICAL_OVERVIEW,
	HLP_SCRN_TACTICAL_MOVEMENT,
	HLP_SCRN_TACTICAL_SIGHT,
	HLP_SCRN_TACTICAL_ATTACKING,
	HLP_SCRN_TACTICAL_ITEMS,
	HLP_SCRN_TACTICAL_KEYBOARD,

	HLP_SCRN_NUM_TACTICAL_PAGES,

};

//ddd


HELP_SCREEN_STRUCT gHelpScreen;


typedef struct
{
	INT32	iButtonTextNum[ HELP_SCREEN_NUM_BTNS ];

} HELP_SCREEN_BTN_TEXT_RECORD;

//An array of record nums for the text on the help buttons
HELP_SCREEN_BTN_TEXT_RECORD gHelpScreenBtnTextRecordNum[ HELP_SCREEN_NUMBER_OF_HELP_SCREENS ] =
{
//new screen:

	//Laptop button record nums
	//	HELP_SCREEN_LAPTOP,
	{ HLP_TXT_LAPTOP_BUTTON_1,
		HLP_TXT_LAPTOP_BUTTON_2,
		HLP_TXT_LAPTOP_BUTTON_3,
		HLP_TXT_LAPTOP_BUTTON_4,
		HLP_TXT_LAPTOP_BUTTON_5,
		HLP_TXT_LAPTOP_BUTTON_6,
		HLP_TXT_LAPTOP_BUTTON_7,
		HLP_TXT_LAPTOP_BUTTON_8, },


//	HELP_SCREEN_MAPSCREEN,
	{ HLP_TXT_WELCOM_TO_ARULCO_BUTTON_1,
		HLP_TXT_WELCOM_TO_ARULCO_BUTTON_2,
		HLP_TXT_WELCOM_TO_ARULCO_BUTTON_3,
		HLP_TXT_WELCOM_TO_ARULCO_BUTTON_4,
		HLP_TXT_WELCOM_TO_ARULCO_BUTTON_5,
		HLP_TXT_WELCOM_TO_ARULCO_BUTTON_6,
		HLP_TXT_WELCOM_TO_ARULCO_BUTTON_7,
		HLP_TXT_WELCOM_TO_ARULCO_BUTTON_8, },

//	HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED,
	{ -1, -1, -1, -1, -1, -1, -1, -1, },

//	HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO,
	{ -1, -1, -1, -1, -1, -1, -1, -1, },

//	HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY,
	{ -1, -1, -1, -1, -1, -1, -1, -1, },

//	HELP_SCREEN_TACTICAL,
	{ HLP_TXT_TACTICAL_BUTTON_1,
		HLP_TXT_TACTICAL_BUTTON_2,
		HLP_TXT_TACTICAL_BUTTON_3,
		HLP_TXT_TACTICAL_BUTTON_4,
		HLP_TXT_TACTICAL_BUTTON_5,
		HLP_TXT_TACTICAL_BUTTON_6,
		-1, -1, },

//	HELP_SCREEN_OPTIONS,
	{ -1, -1, -1, -1, -1, -1, -1, -1, },

//	HELP_SCREEN_LOAD_GAME,
	{ -1, -1, -1, -1, -1, -1, -1, -1, },

};


BOOLEAN	gfHelpScreenEntry = TRUE;
BOOLEAN	gfHelpScreenExit = FALSE;



UINT32	guiHelpScreenBackGround;
UINT32	guiHelpScreenTextBufferSurface;

BOOLEAN	gfScrollBoxIsScrolling = FALSE;

BOOLEAN	gfHaveRenderedFirstFrameToSaveBuffer=FALSE;


//	must use this cause you have ur cursor over a button when entering the help screen, the button will burn though.
//It does this cause that region loses it focus so it draws the button again.
UINT8		gubRenderHelpScreenTwiceInaRow=0;

//mmm

// region to mask the background
MOUSE_REGION	gHelpScreenFullScreenMask;
//void SelectHelpTextFullScreenMaskCallBack(MOUSE_REGION * pRegion, INT32 iReason );


// region to mask the background
MOUSE_REGION	gHelpScreenScrollArea;
void SelectHelpScrollAreaMovementCallBack( MOUSE_REGION * pRegion, INT32 iReason );
void SelectHelpScrollAreaCallBack( MOUSE_REGION * pRegion, INT32 iReason );

// region to mask the background
MOUSE_REGION	gHelpScreenScrollAreaArrows;
void SelectHelpScrollAreaArrowsCallBack( MOUSE_REGION * pRegion, INT32 iReason );


//checkbox to toggle show help again toggle
UINT32	gHelpScreenDontShowHelpAgainToggle;
void BtnHelpScreenDontShowHelpAgainCallback(GUI_BUTTON *btn,INT32 reason);
//MOUSE_REGION	HelpScreenDontShowHelpAgainToggleTextRegion;
//void		HelpScreenDontShowHelpAgainToggleTextRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


INT32		giHelpScreenButtonsImage[ HELP_SCREEN_NUM_BTNS ];
UINT32	guiHelpScreenBtns[ HELP_SCREEN_NUM_BTNS ];
void BtnHelpScreenBtnsCallback(GUI_BUTTON *btn,INT32 reason);

INT32		giExitBtnImage;
UINT32	guiHelpScreenExitBtn;
void BtnHelpScreenExitCallback(GUI_BUTTON *btn,INT32 reason);


INT32		giHelpScreenScrollArrows[2];
UINT32	guiHelpScreenScrollArrowImage[2];
void		BtnHelpScreenScrollArrowsCallback(GUI_BUTTON *btn,INT32 reason);



//ggg





BOOLEAN EnterHelpScreen();
void HandleHelpScreen();
void RenderHelpScreen();
void ExitHelpScreen();

void	GetHelpScreenUserInput();
void HelpScreenSpecialExitCode();
void SetSizeAndPropertiesOfHelpScreen();
BOOLEAN DrawHelpScreenBackGround();
void PrepareToExitHelpScreen();
void SpecialHandlerCode();

UINT16 RenderSpecificHelpScreen();

UINT16 RenderLaptopHelpScreen();
UINT16 RenderTacticalHelpScreen();
UINT16 RenderMapScreenHelpScreen();
UINT16 RenderMapScreenNoOneHiredYetHelpScreen();
UINT16 RenderMapScreenNotYetInArulcoHelpScreen();
UINT16 RenderMapScreenSectorInventoryHelpScreen();

void GetHelpScreenTextPositions( UINT16 *pusPosX, UINT16 *pusPosY, UINT16 *pusWidth );
void DisplayCurrentScreenTitleAndFooter();
void GetHelpScreenText( UINT32 uiRecordToGet, STR16	pText );
UINT16 GetAndDisplayHelpScreenText( UINT32 uiRecord, UINT16 usPosX, UINT16 usPosY, UINT16 usWidth );
void CreateHelpScreenButtons();
void RefreshAllHelpScreenButtons();

void RenderTextBufferToScreen();
void RenderCurrentHelpScreenTextToBuffer();
void DestroyHelpScreenTextBuffer();
BOOLEAN CreateHelpScreenTextBuffer();
void ChangeHelpScreenSubPage();
void ClearHelpScreenTextBuffer();
void ChangeTopLineInTextBufferByAmount( INT32 iAmouontToMove );
void DisplayHelpScreenTextBufferScrollBox();
void CalculateHeightAndPositionForHelpScreenScrollBox( INT32 *piHeightOfScrollBox, INT32 *iTopOfScrollBox );
void HelpScreenMouseMoveScrollBox( INT32 usMousePosY );
void CreateScrollAreaButtons();
void DeleteScrollArrowButtons();
void ChangeToHelpScreenSubPage( INT8 bNewPage );
BOOLEAN AreWeClickingOnScrollBar( INT32 usMousePosY );


//ppp



void InitHelpScreenSystem()
{
	//set some values
	memset( &gHelpScreen, 0, sizeof( gHelpScreen ) );

	//set it up so we can enter the screen
	gfHelpScreenEntry = TRUE;
	gfHelpScreenExit = FALSE;

	gHelpScreen.bCurrentHelpScreenActiveSubPage = -1;

	gHelpScreen.fHaveAlreadyBeenInHelpScreenSinceEnteringCurrenScreen = FALSE;
}

BOOLEAN ShouldTheHelpScreenComeUp( UINT8 ubScreenID, BOOLEAN fForceHelpScreenToComeUp )
{

	//if the screen is being forsced to come up ( user pressed 'h' )
	if( fForceHelpScreenToComeUp )
	{
		//Set thefact that the user broughtthe help screen up
		gHelpScreen.fForceHelpScreenToComeUp = TRUE;

		goto HELP_SCREEN_SHOULD_COME_UP;
	}

	//if we are already in the help system, return true
	if( gHelpScreen.uiFlags & HELP_SCREEN_ACTIVE )
	{
		return( TRUE );
	}

	//has the player been in the screen before
	if( ( gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen >> ubScreenID ) & 0x01 )
	{
		goto HELP_SCREEN_WAIT_1_FRAME;
	}

	//if we have already been in the screen, and the user DIDNT press 'h', leave
	if( gHelpScreen.fHaveAlreadyBeenInHelpScreenSinceEnteringCurrenScreen )
	{
		return( FALSE );
	}

	//should the screen come up, based on the users choice for it automatically coming up
//	if( !( gHelpScreen.fHideHelpInAllScreens ) )
	{
//		goto HELP_SCREEN_WAIT_1_FRAME;
	}

	//the help screen shouldnt come up
	return( FALSE );

HELP_SCREEN_WAIT_1_FRAME:

	// we have to wait 1 frame while the screen renders
	if( gHelpScreen.bDelayEnteringHelpScreenBy1FrameCount < 2 )
	{
		gHelpScreen.bDelayEnteringHelpScreenBy1FrameCount += 1;

		UnmarkButtonsDirty( );

		return( FALSE );
	}

HELP_SCREEN_SHOULD_COME_UP:

	//Record which screen it is

	//if its mapscreen
	if( ubScreenID == HELP_SCREEN_MAPSCREEN )
	{
		//determine which screen it is ( is any mercs hired, did game just start )
		gHelpScreen.bCurrentHelpScreen = HelpScreenDetermineWhichMapScreenHelpToShow();
	}
	else
	{
		gHelpScreen.bCurrentHelpScreen = ubScreenID;
	}

	//mark it that the help screnn is enabled
	gHelpScreen.uiFlags |= HELP_SCREEN_ACTIVE;

	// reset
	gHelpScreen.bDelayEnteringHelpScreenBy1FrameCount = 0;

	return( TRUE );
}


void HelpScreenHandler()
{
	//if we are just entering the help screen
	if( gfHelpScreenEntry )
	{
		//setup the help screen
		EnterHelpScreen();

		gfHelpScreenEntry = FALSE;
		gfHelpScreenExit = FALSE;
	}

	RestoreBackgroundRects();


	//get the mouse and keyboard inputs
	GetHelpScreenUserInput();

	//handle the help screen
	HandleHelpScreen();

	//if the help screen is dirty, re-render it
	if( gHelpScreen.ubHelpScreenDirty != HLP_SCRN_DRTY_LVL_NOT_DIRTY )
	{
		//temp
//		gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_REFRESH_ALL;


		RenderHelpScreen();
		gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_NOT_DIRTY;
	}

	// render buttons marked dirty
//	MarkButtonsDirty( );
	RenderButtons( );

	SaveBackgroundRects( );
	RenderButtonsFastHelp();

	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	//if we are leaving the help screen
	if( gfHelpScreenExit )
	{
		gfHelpScreenExit = FALSE;

		gfHelpScreenEntry = TRUE;

		//exit mouse regions etc..
		ExitHelpScreen();

		//reset the helpscreen id
		gHelpScreen.bCurrentHelpScreen = -1;
	}
}

BOOLEAN EnterHelpScreen()
{
	VOBJECT_DESC	VObjectDesc;
	UINT16 usPosX, usPosY;//, usWidth, usHeight;
//	INT32	iStartLoc;
//	CHAR16 zText[1024];

	//Clear out all the save background rects
	EmptyBackgroundRects( );


	UnmarkButtonsDirty( );

	// remeber if the game was paused or not ( so when we exit we know what to do )
	gHelpScreen.fWasTheGamePausedPriorToEnteringHelpScreen = gfGamePaused;

	//pause the game
	PauseGame( );


	//Determine the help screen size, based off the help screen
	SetSizeAndPropertiesOfHelpScreen();

	//Create a mouse region 'mask' the entrire screen
	MSYS_DefineRegion( &gHelpScreenFullScreenMask, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST,
							gHelpScreen.usCursor, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	MSYS_AddRegion( &gHelpScreenFullScreenMask );


	//Create the exit button
	if( gHelpScreen.bNumberOfButtons != 0 )
		usPosX = gHelpScreen.usScreenLocX + HELP_SCREEN_EXIT_BTN_OFFSET_X + HELP_SCREEN_BUTTON_BORDER_WIDTH;
	else
		usPosX = gHelpScreen.usScreenLocX + HELP_SCREEN_EXIT_BTN_OFFSET_X;

	usPosY = gHelpScreen.usScreenLocY + HELP_SCREEN_EXIT_BTN_LOC_Y;

	//Create the exit buttons
	giExitBtnImage = LoadButtonImage("INTERFACE\\HelpScreen.sti", -1,0,4,2,6 );

	guiHelpScreenExitBtn = CreateIconAndTextButton( giExitBtnImage, L"", HELP_SCREEN_BTN_FONT,
														HELP_SCREEN_BTN_FONT_ON_COLOR, DEFAULT_SHADOW,
														HELP_SCREEN_BTN_FONT_OFF_COLOR, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
														DEFAULT_MOVE_CALLBACK, BtnHelpScreenExitCallback );
	SetButtonFastHelpText( guiHelpScreenExitBtn, gzHelpScreenText[HLP_SCRN_TXT__EXIT_SCREEN] );
	SetButtonCursor( guiHelpScreenExitBtn, gHelpScreen.usCursor);



	//Create the buttons needed for the screen
	CreateHelpScreenButtons();


	//if there are buttons
	if( gHelpScreen.bNumberOfButtons != 0 )
		usPosX = gHelpScreen.usScreenLocX + HELP_SCREEN_SHOW_HELP_AGAIN_REGION_OFFSET_X + HELP_SCREEN_BUTTON_BORDER_WIDTH;
	else
		usPosX = gHelpScreen.usScreenLocX + HELP_SCREEN_SHOW_HELP_AGAIN_REGION_OFFSET_X;

	usPosY = gHelpScreen.usScreenLocY + gHelpScreen.usScreenHeight - HELP_SCREEN_SHOW_HELP_AGAIN_REGION_OFFSET_Y;

	if( !gHelpScreen.fForceHelpScreenToComeUp)
	{
		gHelpScreenDontShowHelpAgainToggle = CreateCheckBoxButton( usPosX, (UINT16)(usPosY-3),
																		"INTERFACE\\OptionsCheckBoxes.sti", MSYS_PRIORITY_HIGHEST,
																		BtnHelpScreenDontShowHelpAgainCallback );

		SetButtonCursor( gHelpScreenDontShowHelpAgainToggle, gHelpScreen.usCursor );

		// Set the state of the chec box
		if( gGameSettings.fHideHelpInAllScreens )
			ButtonList[ gHelpScreenDontShowHelpAgainToggle ]->uiFlags |= BUTTON_CLICKED_ON;
		else
			ButtonList[ gHelpScreenDontShowHelpAgainToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;
	}

/*
	///creatre a region for the text that says ' [ x ] click to continue seeing ....'
	iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_CONSTANT_FOOTER;
	LoadEncryptedDataFromFile(HELPSCREEN_FILE, zText, iStartLoc, HELPSCREEN_RECORD_SIZE );

	usWidth = StringPixLength( zText, HELP_SCREEN_TEXT_BODY_FONT );
	usHeight = GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );

/*
	MSYS_DefineRegion( &HelpScreenDontShowHelpAgainToggleTextRegion, usPosX, usPosY, (UINT16)(usPosX+usWidth), (UINT16)(usPosY+usHeight), MSYS_PRIORITY_HIGHEST-1,
							gHelpScreen.usCursor, MSYS_NO_CALLBACK, HelpScreenDontShowHelpAgainToggleTextRegionCallBack );
	MSYS_AddRegion( &HelpScreenDontShowHelpAgainToggleTextRegion );
*/

	// load the help screen background graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\HelpScreen.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiHelpScreenBackGround));


	//create the text buffer
	CreateHelpScreenTextBuffer();


	//make sure we redraw everything
	gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_REFRESH_ALL;

	//mark it that we have been in since we enter the current screen
	gHelpScreen.fHaveAlreadyBeenInHelpScreenSinceEnteringCurrenScreen = TRUE;

	//set the fact that we have been to the screen
	gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen &= ~( 1 << gHelpScreen.bCurrentHelpScreen );

	//always start at the top
	gHelpScreen.iLineAtTopOfTextBuffer = 0;

	//set it so there was no previous click
	gHelpScreen.iLastMouseClickY = -1;

	//Create the scroll box, and scroll arrow regions/buttons
	CreateScrollAreaButtons();

	//render the active page to the text buffer
	ChangeHelpScreenSubPage();

	//reset scroll box flag
	gfScrollBoxIsScrolling = FALSE;

	//reset first frame buffer
	gfHaveRenderedFirstFrameToSaveBuffer = FALSE;

	gubRenderHelpScreenTwiceInaRow = 0;

	return( TRUE );
}

void HandleHelpScreen()
{
	//if any of the possible screens need to have a some code done every loop..	its done in here
	SpecialHandlerCode();

	if( gfScrollBoxIsScrolling )
	{
		if( gfLeftButtonState )
		{
			HelpScreenMouseMoveScrollBox( gusMouseYPos );
		}
		else
		{
			gfScrollBoxIsScrolling = FALSE;
			gHelpScreen.iLastMouseClickY = -1;
		}
	}

	if( gubRenderHelpScreenTwiceInaRow < 3 )
	{
//test
//		gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_REFRESH_ALL;

		gubRenderHelpScreenTwiceInaRow++;

		UnmarkButtonsDirty( );
	}

	// refresh all of help screens buttons
	RefreshAllHelpScreenButtons();
}

void RenderHelpScreen()
{
//rrr

	if( gfHaveRenderedFirstFrameToSaveBuffer )
	{
		//Restore the background before blitting the text back on
		RestoreExternBackgroundRect( gHelpScreen.usScreenLocX, gHelpScreen.usScreenLocY, gHelpScreen.usScreenWidth, gHelpScreen.usScreenHeight );
	}


	if( gHelpScreen.ubHelpScreenDirty == HLP_SCRN_DRTY_LVL_REFRESH_ALL )
	{
		//Display the helpscreen background
		DrawHelpScreenBackGround();

		//Display the current screens title, and footer info
		DisplayCurrentScreenTitleAndFooter();
	}


	if( !gfHaveRenderedFirstFrameToSaveBuffer )
	{
		gfHaveRenderedFirstFrameToSaveBuffer = TRUE;

		//blit everything to the save buffer ( cause the save buffer can bleed through )
		BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, gHelpScreen.usScreenLocX, gHelpScreen.usScreenLocY, (UINT16)(gHelpScreen.usScreenWidth), (UINT16)(gHelpScreen.usScreenHeight) );

		UnmarkButtonsDirty( );
	}


	//render the text buffer to the screen
	if( gHelpScreen.ubHelpScreenDirty >= HLP_SCRN_DRTY_LVL_REFRESH_TEXT )
	{
		RenderTextBufferToScreen();
	}
}


void ExitHelpScreen()
{
	INT32	i;

	if( !gHelpScreen.fForceHelpScreenToComeUp )
	{
		//Get the current value of the checkbox
		if( ButtonList[ gHelpScreenDontShowHelpAgainToggle ]->uiFlags & BUTTON_CLICKED_ON )
		{
			gGameSettings.fHideHelpInAllScreens = TRUE;
			gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen = 0;
		}
		else
		{
			gGameSettings.fHideHelpInAllScreens = FALSE;
		}

		//remove the mouse region for the '[ ] dont show help...'
		RemoveButton( gHelpScreenDontShowHelpAgainToggle );
	}


	//mark it that the help screen is not active
	gHelpScreen.uiFlags &= ~HELP_SCREEN_ACTIVE;

	//remove the mouse region that blankets
	MSYS_RemoveRegion( &gHelpScreenFullScreenMask );

	//checkbox to toggle show help again toggle
//	MSYS_RemoveRegion( &HelpScreenDontShowHelpAgainToggleTextRegion );


	//remove the hepl graphic
	DeleteVideoObjectFromIndex( guiHelpScreenBackGround );


	//remove the exit button
	RemoveButton( guiHelpScreenExitBtn );

	//if there are any buttons, remove them
	if( gHelpScreen.bNumberOfButtons != 0 )
	{
		for( i=0; i< gHelpScreen.bNumberOfButtons; i++ )
		{
			UnloadButtonImage( giHelpScreenButtonsImage[i] );
			RemoveButton( guiHelpScreenBtns[i] );
		}
	}

	//destroy the text buffer for the help screen
	DestroyHelpScreenTextBuffer();


	//Handles the dirtying of any special screen we are about to reenter
	HelpScreenSpecialExitCode();

	//if the game was NOT paused
	if( gHelpScreen.fWasTheGamePausedPriorToEnteringHelpScreen == FALSE )
	{
		//un pause the game
		UnPauseGame( );
	}

	//Delete the scroll box, and scroll arrow regions/buttons
	DeleteScrollArrowButtons();

	//reset
	gHelpScreen.fForceHelpScreenToComeUp = FALSE;

	SaveGameSettings();
	SaveFeatureFlags();
}


BOOLEAN DrawHelpScreenBackGround()
{
	HVOBJECT hPixHandle;
	UINT16 usPosX;

	//Get and display the background image
	GetVideoObject(&hPixHandle, guiHelpScreenBackGround );

	usPosX = gHelpScreen.usScreenLocX;

	//if there are buttons, blit the button border
	if( gHelpScreen.bNumberOfButtons != 0 )
	{
	BltVideoObject(FRAME_BUFFER, hPixHandle, HLP_SCRN_BUTTON_BORDER, usPosX, gHelpScreen.usScreenLocY, VO_BLT_SRCTRANSPARENCY,NULL);
		usPosX += HELP_SCREEN_BUTTON_BORDER_WIDTH;
	}

	BltVideoObject(FRAME_BUFFER, hPixHandle, HLP_SCRN_DEFAULT_TYPE, usPosX, gHelpScreen.usScreenLocY, VO_BLT_SRCTRANSPARENCY,NULL);

	InvalidateRegion( gHelpScreen.usScreenLocX, gHelpScreen.usScreenLocY, gHelpScreen.usScreenLocX+gHelpScreen.usScreenWidth, gHelpScreen.usScreenLocY + gHelpScreen.usScreenHeight );

	return( TRUE );
}

void SetSizeAndPropertiesOfHelpScreen()
{


//new screen:
	gHelpScreen.bNumberOfButtons = 0;

	//
	//these are the default settings, so if the screen uses different then defualt, set them in the switch
	//
	{
		gHelpScreen.usScreenWidth = HELP_SCREEN_DEFUALT_LOC_WIDTH;
		gHelpScreen.usScreenHeight = HELP_SCREEN_DEFUALT_LOC_HEIGHT;

		gHelpScreen.usScreenLocX = ( SCREEN_WIDTH - gHelpScreen.usScreenWidth ) / 2;
		gHelpScreen.usScreenLocY = ( SCREEN_HEIGHT - gHelpScreen.usScreenHeight ) / 2;

		gHelpScreen.bCurrentHelpScreenActiveSubPage = 0;

				gHelpScreen.usCursor = CURSOR_NORMAL;
	}


	switch( gHelpScreen.bCurrentHelpScreen )
	{
		case HELP_SCREEN_LAPTOP:
			gHelpScreen.bNumberOfButtons = HLP_SCRN_LPTP_NUM_PAGES;
			gHelpScreen.usCursor = CURSOR_LAPTOP_SCREEN;

			//center the screen inside the laptop screen
			gHelpScreen.usScreenLocX = LAPTOP_SCREEN_UL_X + ( 502 - gHelpScreen.usScreenWidth ) / 2;
			gHelpScreen.usScreenLocY = LAPTOP_SCREEN_UL_Y + ( LAPTOP_SCREEN_HEIGHT - gHelpScreen.usScreenHeight ) / 2;

			break;
		case HELP_SCREEN_MAPSCREEN:
			gHelpScreen.bNumberOfButtons = HLP_SCRN_NUM_MPSCRN_BTNS;

			//calc the center position based on the current panel thats being displayed
			gHelpScreen.usScreenLocY = ( gsVIEWPORT_END_Y - gHelpScreen.usScreenHeight ) / 2;
			break;
		case HELP_SCREEN_TACTICAL:
			gHelpScreen.bNumberOfButtons = HLP_SCRN_NUM_TACTICAL_PAGES;

			//calc the center position based on the current panel thats being displayed
			gHelpScreen.usScreenLocY = ( gsVIEWPORT_END_Y - gHelpScreen.usScreenHeight ) / 2;
			break;

		case HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED:
		case HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO:
		case HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY:
			gHelpScreen.usScreenWidth = HELP_SCREEN_SMALL_LOC_WIDTH;
			gHelpScreen.usScreenHeight = HELP_SCREEN_SMALL_LOC_HEIGHT;

			//calc screen position since we just set the width and height
			gHelpScreen.usScreenLocX = ( SCREEN_WIDTH - gHelpScreen.usScreenWidth ) / 2;

			//calc the center position based on the current panel thats being displayed
			gHelpScreen.usScreenLocY = ( gsVIEWPORT_END_Y - gHelpScreen.usScreenHeight ) / 2;

			gHelpScreen.bNumberOfButtons = 0;
			gHelpScreen.bCurrentHelpScreenActiveSubPage = 0;
			break;


		case HELP_SCREEN_OPTIONS:
		case HELP_SCREEN_LOAD_GAME:
			break;

		default:
			#ifdef JA2BETAVERSION
				AssertMsg( 0, "Error in help screen.	DF 0" );
		#else
		break;
			#endif
	}

	//if there are buttons
	if( gHelpScreen.bNumberOfButtons != 0 )
		gHelpScreen.usLeftMarginPosX	= gHelpScreen.usScreenLocX + HELP_SCREEN_TEXT_LEFT_MARGIN_WITH_BTN;
	else
		gHelpScreen.usLeftMarginPosX	= gHelpScreen.usScreenLocX + HELP_SCREEN_TEXT_LEFT_MARGIN;

}

void CreateHelpScreenButtons()
{
	UINT16 usPosX, usPosY;
	CHAR16	sText[1024];
	INT32	i;

	//if there are buttons to create
	if( gHelpScreen.bNumberOfButtons != 0 )
	{

		usPosX = gHelpScreen.usScreenLocX + HELP_SCREEN_BTN_OFFSET_X;
		usPosY = HELP_SCREEN_BTN_OFFSET_Y + gHelpScreen.usScreenLocY;


		//loop through all the buttons, and create them
		for( i=0; i< gHelpScreen.bNumberOfButtons; i++ )
		{
			//get the text for the button
			GetHelpScreenText( gHelpScreenBtnTextRecordNum[ gHelpScreen.bCurrentHelpScreen ].iButtonTextNum[i], sText );

/*
			guiHelpScreenBtns[i] = CreateTextButton( sText, HELP_SCREEN_BTN_FONT, HELP_SCREEN_BTN_FONT_COLOR, HELP_SCREEN_BTN_FONT_BACK_COLOR,
					BUTTON_USE_DEFAULT, usPosX, usPosY, HELP_SCREEN_BTN_WIDTH, HELP_SCREEN_BTN_HEIGHT,
					BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST, BUTTON_NO_CALLBACK, BtnHelpScreenBtnsCallback );
*/


			giHelpScreenButtonsImage[i] = UseLoadedButtonImage( giExitBtnImage, -1,1,5,3,7 );

			guiHelpScreenBtns[i] = CreateIconAndTextButton( giHelpScreenButtonsImage[i], sText, HELP_SCREEN_BTN_FONT,
															HELP_SCREEN_BTN_FONT_ON_COLOR, DEFAULT_SHADOW,
															HELP_SCREEN_BTN_FONT_OFF_COLOR, DEFAULT_SHADOW,
															TEXT_CJUSTIFIED,
															usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
															DEFAULT_MOVE_CALLBACK, BtnHelpScreenBtnsCallback);


			SetButtonCursor( guiHelpScreenBtns[i], gHelpScreen.usCursor);
			MSYS_SetBtnUserData( guiHelpScreenBtns[i], 0, i);

//	SpecifyButtonTextOffsets( guiHelpScreenBtns[i], 19, 9, TRUE );

			usPosY += HELP_SCREEN_BTN_HEIGHT + HELP_SCREEN_GAP_BN_BTNS;
		}

		ButtonList[ guiHelpScreenBtns[0] ]->uiFlags |= BUTTON_CLICKED_ON;
	}
}


void	GetHelpScreenUserInput()
{
	InputAtom Event;
	POINT	MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	while( DequeueEvent( &Event ) )
	{
		// HOOK INTO MOUSE HOOKS
		switch( Event.usEvent)
	{
			case LEFT_BUTTON_DOWN:
				MouseSystemHook(LEFT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_UP:
				MouseSystemHook(LEFT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y ,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_DOWN:
				MouseSystemHook(RIGHT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_UP:
				MouseSystemHook(RIGHT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_REPEAT:
				MouseSystemHook(RIGHT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_REPEAT:
				MouseSystemHook(LEFT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
		}


		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_UP )
		{
			switch( Event.usParam )
			{
				case ESC:
					PrepareToExitHelpScreen();
					break;

				case DNARROW:
				{
					ChangeTopLineInTextBufferByAmount( 1 );
				}
				break;

				case UPARROW:
				{
					ChangeTopLineInTextBufferByAmount( -1 );
				}
				break;

				case PGUP:
				{
					ChangeTopLineInTextBufferByAmount( -( HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER -1 ) );
				}
				break;
				case PGDN:
				{
					ChangeTopLineInTextBufferByAmount( ( HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER -1 ) );
				}
				break;

				case LEFTARROW:
					ChangeToHelpScreenSubPage( (INT8)( gHelpScreen.bCurrentHelpScreenActiveSubPage - 1 ) );
					break;

				case RIGHTARROW:
					ChangeToHelpScreenSubPage( (INT8)( gHelpScreen.bCurrentHelpScreenActiveSubPage + 1 ) );
					break;

/*

				case LEFTARROW:
				{
				}
					break;

				case RIGHTARROW:
				{
				}
					break;
*/


#ifdef JA2TESTVERSION
				//rerender the hepl screen
				case 'r':
					gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_REFRESH_ALL;
					break;

				case 'i':
					InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
					break;

				case 'd':
				InvalidateRegion( gHelpScreen.usScreenLocX, gHelpScreen.usScreenLocY, gHelpScreen.usScreenLocX+gHelpScreen.usScreenWidth, gHelpScreen.usScreenLocY + gHelpScreen.usScreenHeight );
					break;
#endif
			}
		}

		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_REPEAT )
		{
			switch( Event.usParam )
			{
				case DNARROW:
				{
					ChangeTopLineInTextBufferByAmount( 1 );
				}
				break;

				case UPARROW:
				{
					ChangeTopLineInTextBufferByAmount( -1 );
				}
				break;

				case PGUP:
				{
					ChangeTopLineInTextBufferByAmount( -( HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER -1 ) );
				}
				break;
				case PGDN:
				{
					ChangeTopLineInTextBufferByAmount( ( HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER -1 ) );
				}
				break;
			}
		}
	}
}


//Handles anything spcial that must be done when exiting the specific screen we are about to reenter ( eg. dirtying of the screen )
void HelpScreenSpecialExitCode()
{
	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreen )
	{
		case HELP_SCREEN_LAPTOP:
			fReDrawScreenFlag = TRUE;
			break;

		case HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED:
		case HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO:
		case HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY:
		case HELP_SCREEN_MAPSCREEN:
			fCharacterInfoPanelDirty = TRUE;
			fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fMapPanelDirty=TRUE;
			break;

		case HELP_SCREEN_TACTICAL:
			fInterfacePanelDirty = DIRTYLEVEL2;
			SetRenderFlags(RENDER_FLAG_FULL);
			break;

		case HELP_SCREEN_OPTIONS:
			break;
		case HELP_SCREEN_LOAD_GAME:
			break;

		default:
			#ifdef JA2BETAVERSION
				AssertMsg( 0, "Error in help screen.	DF 0" );
		#else
		break;
			#endif
	}
}

void PrepareToExitHelpScreen()
{
	gfHelpScreenExit = TRUE;
}


//Handles anything special that must be done when exiting the specific screen we are about to reenter ( eg. dirtying of the screen )
void SpecialHandlerCode()
{
	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreen )
	{
		case HELP_SCREEN_LAPTOP:
			PrintDate( );
			PrintBalance( );
			PrintNumberOnTeam( );
			break;
		case HELP_SCREEN_MAPSCREEN:
			break;
		case HELP_SCREEN_TACTICAL:
			break;

		case HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED:
			break;
		case HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO:
			break;
		case HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY:
			break;
			break;
		case HELP_SCREEN_OPTIONS:
			break;
		case HELP_SCREEN_LOAD_GAME:
			break;

		default:
			#ifdef JA2BETAVERSION
				AssertMsg( 0, "Error in help screen:	SpecialHandlerCode().	DF 0" );
		#else
		break;
			#endif
	}
}


UINT16 RenderSpecificHelpScreen()
{
	UINT16	usNumVerticalPixelsDisplayed = 0;
//new screen:

	//set the buffer for the text to go to
//	SetFontDestBuffer( guiHelpScreenTextBufferSurface, gHelpScreen.usLeftMarginPosX, gHelpScreen.usScreenLocY + HELP_SCREEN_TEXT_OFFSET_Y,
//										HLP_SCRN__WIDTH_OF_TEXT_BUFFER, HLP_SCRN__NUMBER_BYTES_IN_TEXT_BUFFER, FALSE );
	SetFontDestBuffer( guiHelpScreenTextBufferSurface, 0, 0,
										HLP_SCRN__WIDTH_OF_TEXT_BUFFER, HLP_SCRN__HEIGHT_OF_TEXT_BUFFER, FALSE );


	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreen )
	{
		case HELP_SCREEN_LAPTOP:
			usNumVerticalPixelsDisplayed = RenderLaptopHelpScreen();
			break;
		case HELP_SCREEN_MAPSCREEN:
			usNumVerticalPixelsDisplayed = RenderMapScreenHelpScreen();
			break;
		case HELP_SCREEN_TACTICAL:
			usNumVerticalPixelsDisplayed = RenderTacticalHelpScreen();
			break;
		case HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED:
			usNumVerticalPixelsDisplayed = RenderMapScreenNoOneHiredYetHelpScreen();
			break;
		case HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO:
			usNumVerticalPixelsDisplayed = RenderMapScreenNotYetInArulcoHelpScreen();
			break;
		case HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY:
			usNumVerticalPixelsDisplayed = RenderMapScreenSectorInventoryHelpScreen();
			break;
		case HELP_SCREEN_OPTIONS:
			break;
		case HELP_SCREEN_LOAD_GAME:
			break;

		default:
			#ifdef JA2BETAVERSION
				SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
				AssertMsg( 0, "Error in help screen:	RenderSpecificHelpScreen().	DF 0" );
		#else
		break;
			#endif
	}

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	//add 1 line to the bottom of the buffer
	usNumVerticalPixelsDisplayed += 10;

	return( usNumVerticalPixelsDisplayed );
}

void GetHelpScreenTextPositions( UINT16 *pusPosX, UINT16 *pusPosY, UINT16 *pusWidth )
{
	//if there are buttons
	if( pusPosX != NULL )
		*pusPosX = 0;

	if( pusWidth != NULL )
		*pusWidth = HLP_SCRN__WIDTH_OF_TEXT_BUFFER - 1 * HELP_SCREEN_MARGIN_SIZE;		//DEF was 2

	if( pusPosY != NULL )
		*pusPosY = 0;
}



void DisplayCurrentScreenTitleAndFooter()
{
	INT32	iStartLoc = -1;
	CHAR16	zText[1024];
	UINT16	usPosX=0, usPosY=0, usWidth=0;

//new screen:

	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreen )
	{
		case HELP_SCREEN_LAPTOP:
			iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_LAPTOP_TITLE;
			break;
		case HELP_SCREEN_MAPSCREEN:
			iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_WELCOM_TO_ARULCO_TITLE;
			break;
		case HELP_SCREEN_TACTICAL:
			iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_TACTICAL_TITLE;
			break;
		case HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED:
			iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_MPSCRN_NO_1_HIRED_YET_TITLE;
			break;
		case HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO:
			iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_MPSCRN_NOT_IN_ARULCO_TITLE;
			break;
		case HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY:
			iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_SECTOR_INVTRY_TITLE;
			break;
		case HELP_SCREEN_OPTIONS:
			break;
		case HELP_SCREEN_LOAD_GAME:
			break;

		default:
			#ifdef JA2BETAVERSION
				AssertMsg( 0, "Error in help screen:	DisplayCurrentScreenTitleAndFooter().	DF 0" );
		#else
		break;
			#endif
	}

//	GetHelpScreenTextPositions( NULL, NULL, &usWidth );

	if( gHelpScreen.bNumberOfButtons != 0 )
		usWidth = gHelpScreen.usScreenWidth - HELP_SCREEN_TEXT_LEFT_MARGIN_WITH_BTN - HELP_SCREEN_TEXT_RIGHT_MARGIN_SPACE;
	else
		usWidth = gHelpScreen.usScreenWidth - HELP_SCREEN_TEXT_LEFT_MARGIN - HELP_SCREEN_TEXT_RIGHT_MARGIN_SPACE;

	//if this screen has a valid title
	if( iStartLoc != -1 )
	{
		LoadEncryptedDataFromFile(HELPSCREEN_FILE, zText, iStartLoc, HELPSCREEN_RECORD_SIZE );

		SetFontShadow( NO_SHADOW );

		usPosX = gHelpScreen.usLeftMarginPosX;

//		DrawTextToScreen( zText, usPosX, (UINT16)(gHelpScreen.usScreenLocY+HELP_SCREEN_TITLE_OFFSET_Y), usWidth,
//									HELP_SCREEN_TITLE_BODY_FONT, HELP_SCREEN_TITLE_BODY_COLOR, HELP_SCREEN_TEXT_BACKGROUND, FALSE, CENTER_JUSTIFIED );

		//Display the Title
		IanDisplayWrappedString( usPosX, (UINT16)(gHelpScreen.usScreenLocY+HELP_SCREEN_TITLE_OFFSET_Y), usWidth, HELP_SCREEN_GAP_BTN_LINES,
														HELP_SCREEN_TITLE_BODY_FONT, HELP_SCREEN_TITLE_BODY_COLOR, zText,
														HELP_SCREEN_TEXT_BACKGROUND, FALSE, 0 );

	}

	//Display the '( press H to get help... )'
	iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_CONSTANT_SUBTITLE;
	LoadEncryptedDataFromFile(HELPSCREEN_FILE, zText, iStartLoc, HELPSCREEN_RECORD_SIZE );

	usPosX = gHelpScreen.usLeftMarginPosX;

	usPosY = gHelpScreen.usScreenLocY+HELP_SCREEN_HELP_REMINDER_Y;
//	DrawTextToScreen( zText, usPosX, usPosY, usWidth,
//								HELP_SCREEN_TEXT_BODY_FONT, HELP_SCREEN_TITLE_BODY_COLOR, HELP_SCREEN_TEXT_BACKGROUND, FALSE, CENTER_JUSTIFIED );


	IanDisplayWrappedString( usPosX, usPosY, usWidth, HELP_SCREEN_GAP_BTN_LINES,
													HELP_SCREEN_TITLE_BODY_FONT, HELP_SCREEN_TITLE_BODY_COLOR, zText,
													HELP_SCREEN_TEXT_BACKGROUND, FALSE, 0 );

	if( !gHelpScreen.fForceHelpScreenToComeUp )
	{
		//calc location for the ' [ x ] Dont display again...'
		iStartLoc = HELPSCREEN_RECORD_SIZE * HLP_TXT_CONSTANT_FOOTER;
		LoadEncryptedDataFromFile(HELPSCREEN_FILE, zText, iStartLoc, HELPSCREEN_RECORD_SIZE );

		usPosX = gHelpScreen.usLeftMarginPosX + HELP_SCREEN_SHOW_HELP_AGAIN_REGION_TEXT_OFFSET_X;

		usPosY = gHelpScreen.usScreenLocY + gHelpScreen.usScreenHeight - HELP_SCREEN_SHOW_HELP_AGAIN_REGION_TEXT_OFFSET_Y + 2;


		//Display the ' [ x ] Dont display again...'
		IanDisplayWrappedString( usPosX, usPosY, usWidth, HELP_SCREEN_GAP_BTN_LINES,
														HELP_SCREEN_TEXT_BODY_FONT, HELP_SCREEN_TITLE_BODY_COLOR, zText,
														HELP_SCREEN_TEXT_BACKGROUND, FALSE, 0 );
	}

	SetFontShadow( DEFAULT_SHADOW );
}

void BtnHelpScreenBtnsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
//		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		//Get the btn id
		INT8	bRetValue = (UINT8)MSYS_GetBtnUserData( btn, 0 );

		ChangeToHelpScreenSubPage( bRetValue );
/*
		//change the current page to the new one
		gHelpScreen.bCurrentHelpScreenActiveSubPage = ( bRetValue > gHelpScreen.bNumberOfButtons ) ? gHelpScreen.bNumberOfButtons-1 : bRetValue;

		gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_REFRESH_TEXT;

		for( i=0; i< gHelpScreen.bNumberOfButtons; i++ )
		{
			ButtonList[ guiHelpScreenBtns[i] ]->uiFlags &= (~BUTTON_CLICKED_ON );
		}

		//change the current sub page, and render it to the buffer
		ChangeHelpScreenSubPage();
*/
		btn->uiFlags |= BUTTON_CLICKED_ON;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);

	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
//		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void ChangeToHelpScreenSubPage( INT8 bNewPage )
{
	INT8 i;

	//if for some reason, we are assigning a lower number
	if( bNewPage < 0 )
	{
		gHelpScreen.bCurrentHelpScreenActiveSubPage = 0;
	}

	//for some reason if the we are passing in a # that is greater then the max, set it to the max
	else if( bNewPage >= gHelpScreen.bNumberOfButtons )
	{
		gHelpScreen.bCurrentHelpScreenActiveSubPage = ( gHelpScreen.bNumberOfButtons == 0 ) ? 0: gHelpScreen.bNumberOfButtons -1;
	}

	//if we are selecting the current su page, exit
	else if( bNewPage == gHelpScreen.bCurrentHelpScreenActiveSubPage )
	{
		return;
	}

	//else assign the new subpage
	else
	{
		gHelpScreen.bCurrentHelpScreenActiveSubPage = bNewPage;
	}

	//refresh the screen
	gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_REFRESH_TEXT;

	//'undepress' all the buttons
	for( i=0; i< gHelpScreen.bNumberOfButtons; i++ )
	{
		ButtonList[ guiHelpScreenBtns[i] ]->uiFlags &= (~BUTTON_CLICKED_ON );
	}

	//depress the proper button
	ButtonList[ guiHelpScreenBtns[ gHelpScreen.bCurrentHelpScreenActiveSubPage ] ]->uiFlags |= BUTTON_CLICKED_ON;

	//change the current sub page, and render it to the buffer
	ChangeHelpScreenSubPage();
}

void GetHelpScreenText( UINT32 uiRecordToGet, STR16	pText )
{
	INT32	iStartLoc = -1;

	iStartLoc = HELPSCREEN_RECORD_SIZE * uiRecordToGet;
	LoadEncryptedDataFromFile(HELPSCREEN_FILE, pText, iStartLoc, HELPSCREEN_RECORD_SIZE );
}

//returns the number of vertical pixels printed
UINT16 GetAndDisplayHelpScreenText( UINT32 uiRecord, UINT16 usPosX, UINT16 usPosY, UINT16 usWidth )
{
	CHAR16	zText[1024];
	UINT16	usNumVertPixels = 0;
	UINT32 uiStartLoc;

	SetFontShadow( NO_SHADOW );

	GetHelpScreenText( uiRecord, zText );

	//Get the record
	uiStartLoc = HELPSCREEN_RECORD_SIZE * uiRecord;
	LoadEncryptedDataFromFile(HELPSCREEN_FILE, zText, uiStartLoc, HELPSCREEN_RECORD_SIZE );

	//Display the text
	usNumVertPixels = IanDisplayWrappedString( usPosX, usPosY, usWidth, HELP_SCREEN_GAP_BTN_LINES,
													HELP_SCREEN_TEXT_BODY_FONT, HELP_SCREEN_TEXT_BODY_COLOR, zText,
													HELP_SCREEN_TEXT_BACKGROUND, FALSE, 0 );

	SetFontShadow( DEFAULT_SHADOW );

	return( usNumVertPixels );
}





void BtnHelpScreenDontShowHelpAgainCallback( GUI_BUTTON *btn, INT32 reason )
{
//	UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
/*
		btn->uiFlags &= ~BUTTON_CLICKED_ON;

		if( gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen & ( 1 << gHelpScreen.bCurrentHelpScreen ) )
		{
//
			gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen &= ~( 1 << gHelpScreen.bCurrentHelpScreen );
		}
		else
		{
//			gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen |= ( 1 << gHelpScreen.bCurrentHelpScreen );

		}
//		btn->uiFlags |= BUTTON_CLICKED_ON;
*/
	}
}

/*
void HelpScreenDontShowHelpAgainToggleTextRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}


	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( gGameSettings.fOptions[ ubButton ] )
		{
		}
		else
		{
		}
	}
}
*/

//set the fact the we have chmaged to a new screen
void NewScreenSoResetHelpScreen( )
{
	gHelpScreen.fHaveAlreadyBeenInHelpScreenSinceEnteringCurrenScreen = FALSE;
	gHelpScreen.bDelayEnteringHelpScreenBy1FrameCount = 0;
}


void BtnHelpScreenExitCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);

		PrepareToExitHelpScreen();

		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


UINT16 RenderLaptopHelpScreen()
{
	UINT16	usPosX, usPosY, usWidth, usNumVertPixels;
	UINT8		ubCnt;
	UINT16	usTotalNumberOfVerticalPixels=0;
	UINT16	usFontHeight = GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );


	if( gHelpScreen.bCurrentHelpScreenActiveSubPage == -1 )
	{
		return( 0 );
	}

	//Get the position for the text
	GetHelpScreenTextPositions( &usPosX, &usPosY, &usWidth );

	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreenActiveSubPage )
	{
		case HLP_SCRN_LPTP_OVERVIEW:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<2; ubCnt++ )
			{
				//Display the text, and get the number of pixels it used to display it
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_OVERVIEW_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

/*
			//Display the first paragraph
			usTotalNumberOfVerticalPixels = GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_OVERVIEW_P1, usPosX, usPosY, usWidth );

			usPosY = usPosY+ usNumVertPixels + GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );

			//Display the second paragraph
			usTotalNumberOfVerticalPixels += GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_OVERVIEW_P2, usPosX, usPosY, usWidth );
*/
			break;

		case HLP_SCRN_LPTP_EMAIL:

			//Display the first paragraph
			usTotalNumberOfVerticalPixels = GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_EMAIL_P1, usPosX, usPosY, usWidth );
			break;


		case HLP_SCRN_LPTP_WEB:

			//Display the first paragraph
			usTotalNumberOfVerticalPixels = GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_WEB_P1, usPosX, usPosY, usWidth );

			break;


		case HLP_SCRN_LPTP_FILES:

			//Display the first paragraph
			usTotalNumberOfVerticalPixels = GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_FILES_P1, usPosX, usPosY, usWidth );
			break;


		case HLP_SCRN_LPTP_HISTORY:
			//Display the first paragraph
			usTotalNumberOfVerticalPixels = GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_HISTORY_P1, usPosX, usPosY, usWidth );

			break;


		case HLP_SCRN_LPTP_PERSONNEL:

			//Display the first paragraph
			usTotalNumberOfVerticalPixels = GetAndDisplayHelpScreenText( HLP_TXT_LAPTOP_PERSONNEL_P1, usPosX, usPosY, usWidth );
			break;

		case HLP_SCRN_LPTP_FINANCIAL:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<2; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_FINANCES_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;

		case HLP_SCRN_LPTP_MERC_STATS:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<15; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_MERC_STATS_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;
	}

	return( usTotalNumberOfVerticalPixels );
}


UINT16 RenderMapScreenNoOneHiredYetHelpScreen()
{
	UINT16	usPosX, usPosY, usWidth, usNumVertPixels;
	UINT8		ubCnt;
	UINT16	usTotalNumberOfVerticalPixels=0;
	UINT16	usFontHeight = GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );


	if( gHelpScreen.bCurrentHelpScreenActiveSubPage == -1 )
	{
		return( 0 );
	}

	//Get the position for the text
	GetHelpScreenTextPositions( &usPosX, &usPosY, &usWidth );

	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreenActiveSubPage )
	{
		case HLP_SCRN_NO_ONE_HIRED:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<2; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_MPSCRN_NO_1_HIRED_YET_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;
	}

	return( usTotalNumberOfVerticalPixels );
}


UINT16 RenderMapScreenNotYetInArulcoHelpScreen()
{
	UINT16	usPosX, usPosY, usWidth, usNumVertPixels;
	UINT8		ubCnt;
	UINT16	usTotalNumberOfVerticalPixels=0;
	UINT16	usFontHeight = GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );


	if( gHelpScreen.bCurrentHelpScreenActiveSubPage == -1 )
	{
		return( 0 );
	}

	//Get the position for the text
	GetHelpScreenTextPositions( &usPosX, &usPosY, &usWidth );

	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreenActiveSubPage )
	{
		case HLP_SCRN_NOT_IN_ARULCO:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<3; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_MPSCRN_NOT_IN_ARULCO_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;
	}

	return( usTotalNumberOfVerticalPixels );
}


UINT16 RenderMapScreenSectorInventoryHelpScreen()
{
	UINT16	usPosX, usPosY, usWidth, usNumVertPixels;
	UINT8		ubCnt;
	UINT16	usTotalNumberOfVerticalPixels=0;
	UINT16	usFontHeight = GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );


	if( gHelpScreen.bCurrentHelpScreenActiveSubPage == -1 )
	{
		return( 0 );
	}

	//Get the position for the text
	GetHelpScreenTextPositions( &usPosX, &usPosY, &usWidth );

	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreenActiveSubPage )
	{
		case HLP_SCRN_MPSCRN_SCTR_OVERVIEW:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<2; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_SECTOR_INVTRY_OVERVIEW_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;
	}

	return( usTotalNumberOfVerticalPixels );
}




UINT16 RenderTacticalHelpScreen()
{
	UINT16	usPosX, usPosY, usWidth, usNumVertPixels;
	UINT8		ubCnt;
	UINT16	usTotalNumberOfVerticalPixels=0;
	UINT16	usFontHeight = GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );


	if( gHelpScreen.bCurrentHelpScreenActiveSubPage == -1 )
	{
		return( 0 );
	}

	//Get the position for the text
	GetHelpScreenTextPositions( &usPosX, &usPosY, &usWidth );

	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreenActiveSubPage )
	{
		case HLP_SCRN_TACTICAL_OVERVIEW:

			//Display all the paragraph
			for( ubCnt=0; ubCnt<4; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_TACTICAL_OVERVIEW_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;

		case HLP_SCRN_TACTICAL_MOVEMENT:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<4; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_TACTICAL_MOVEMENT_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;


		case HLP_SCRN_TACTICAL_SIGHT:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<4; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_TACTICAL_SIGHT_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;


		case HLP_SCRN_TACTICAL_ATTACKING:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<3; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_TACTICAL_ATTACKING_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;


		case HLP_SCRN_TACTICAL_ITEMS:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<4; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_TACTICAL_ITEMS_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;


		case HLP_SCRN_TACTICAL_KEYBOARD:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<8; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_TACTICAL_KEYBOARD_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;
	}

	return( usTotalNumberOfVerticalPixels );
}



UINT16 RenderMapScreenHelpScreen()
{
	UINT16	usPosX, usPosY, usWidth, usNumVertPixels;
	UINT8		ubCnt;
	UINT16	usTotalNumberOfVerticalPixels=0;
	UINT16	usFontHeight = GetFontHeight( HELP_SCREEN_TEXT_BODY_FONT );


	if( gHelpScreen.bCurrentHelpScreenActiveSubPage == -1 )
	{
		return( 0 );
	}

	//Get the position for the text
	GetHelpScreenTextPositions( &usPosX, &usPosY, &usWidth );

	//switch on the current screen
	switch( gHelpScreen.bCurrentHelpScreenActiveSubPage )
	{
		case HLP_SCRN_MPSCRN_OVERVIEW:

			//Display all the paragraph
			for( ubCnt=0; ubCnt<3; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_OVERVIEW_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;

		case HLP_SCRN_MPSCRN_ASSIGNMENTS:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<4; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_ASSNMNT_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;


		case HLP_SCRN_MPSCRN_DESTINATIONS:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<5; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_DSTINATION_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;


		case HLP_SCRN_MPSCRN_MAP:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<3; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_MAP_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;


		case HLP_SCRN_MPSCRN_MILITIA:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<3; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_MILITIA_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}

			break;


		case HLP_SCRN_MPSCRN_AIRSPACE:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<2; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_AIRSPACE_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;

		case HLP_SCRN_MPSCRN_ITEMS:

			//Display all the paragraphs
			for( ubCnt=0; ubCnt<1; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_ITEMS_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;

		case HLP_SCRN_MPSCRN_KEYBOARD:
			//Display all the paragraphs
			for( ubCnt=0; ubCnt<4; ubCnt++ )
			{
				usNumVertPixels = GetAndDisplayHelpScreenText( HLP_TXT_WELCOM_TO_ARULCO_KEYBOARD_P1+ubCnt, usPosX, usPosY, usWidth );

				//move the next text down by the right amount
				usPosY = usPosY+ usNumVertPixels + usFontHeight;

				//add the total amount of pixels used
				usTotalNumberOfVerticalPixels += usNumVertPixels + usFontHeight;
			}
			break;
	}

	return( usTotalNumberOfVerticalPixels );
}


void RefreshAllHelpScreenButtons()
{
	UINT8 i;

		//loop through all the buttons, and refresh them
	for( i=0; i< gHelpScreen.bNumberOfButtons; i++ )
	{
		ButtonList[ guiHelpScreenBtns[i] ]->uiFlags |= BUTTON_DIRTY;
	}

	ButtonList[ guiHelpScreenExitBtn ]->uiFlags |= BUTTON_DIRTY;

	if( !gHelpScreen.fForceHelpScreenToComeUp )
	{
		ButtonList[ gHelpScreenDontShowHelpAgainToggle ]->uiFlags |= BUTTON_DIRTY;
	}

	ButtonList[ giHelpScreenScrollArrows[0] ]->uiFlags |= BUTTON_DIRTY;
	ButtonList[ giHelpScreenScrollArrows[1] ]->uiFlags |= BUTTON_DIRTY;
}



INT8 HelpScreenDetermineWhichMapScreenHelpToShow()
{
	if( fShowMapInventoryPool )
	{
		return( HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY );
	}

	if( AnyMercsHired() == FALSE )
	{
		return( HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED );
	}

	if( gTacticalStatus.fDidGameJustStart )
	{
		return( HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO );
	}

	return( HELP_SCREEN_MAPSCREEN );
}


BOOLEAN CreateHelpScreenTextBuffer()
{
	VSURFACE_DESC		vs_desc;

	// Create a background video surface to blt the face onto
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = HLP_SCRN__WIDTH_OF_TEXT_BUFFER;
	vs_desc.usHeight = HLP_SCRN__HEIGHT_OF_TEXT_BUFFER;
	vs_desc.ubBitDepth = 16;
	CHECKF( AddVideoSurface( &vs_desc, &guiHelpScreenTextBufferSurface) );

	return( TRUE );
}

void DestroyHelpScreenTextBuffer()
{
	DeleteVideoSurfaceFromIndex( guiHelpScreenTextBufferSurface );
}

void RenderCurrentHelpScreenTextToBuffer()
{
	//clear the buffer ( use 0, black as a transparent color
	ClearHelpScreenTextBuffer();

	//Render the current screen, and get the number of pixels it used to display
	gHelpScreen.usTotalNumberOfPixelsInBuffer = RenderSpecificHelpScreen();

	//calc the number of lines in the buffer
	gHelpScreen.usTotalNumberOfLinesInBuffer = gHelpScreen.usTotalNumberOfPixelsInBuffer / ( HLP_SCRN__HEIGHT_OF_1_LINE_IN_BUFFER );
}


void RenderTextBufferToScreen()
{
	HVSURFACE hDestVSurface, hSrcVSurface;
	SGPRect		SrcRect;


	GetVideoSurface( &hDestVSurface, guiRENDERBUFFER );
	GetVideoSurface( &hSrcVSurface, guiHelpScreenTextBufferSurface );

	SrcRect.iLeft = 0;
	SrcRect.iTop = gHelpScreen.iLineAtTopOfTextBuffer * HLP_SCRN__HEIGHT_OF_1_LINE_IN_BUFFER;
	SrcRect.iRight = HLP_SCRN__WIDTH_OF_TEXT_BUFFER;
	SrcRect.iBottom = SrcRect.iTop + HLP_SCRN__HEIGHT_OF_TEXT_AREA - ( 2 * 8 );

	BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, VO_BLT_SRCTRANSPARENCY, gHelpScreen.usLeftMarginPosX, ( gHelpScreen.usScreenLocY + HELP_SCREEN_TEXT_OFFSET_Y ), (RECT*)&SrcRect );

	DisplayHelpScreenTextBufferScrollBox();
}

void ChangeHelpScreenSubPage()
{
	//reset
	gHelpScreen.iLineAtTopOfTextBuffer = 0;

	RenderCurrentHelpScreenTextToBuffer();

	//enable or disable the help screen arrow buttons
	if( gHelpScreen.usTotalNumberOfLinesInBuffer <= HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER )
	{
		DisableButton( giHelpScreenScrollArrows[0] );
		DisableButton( giHelpScreenScrollArrows[1] );
	}
	else
	{
		EnableButton( giHelpScreenScrollArrows[0] );
		EnableButton( giHelpScreenScrollArrows[1] );
	}
}

void ClearHelpScreenTextBuffer()
{
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;

	// CLEAR THE FRAME BUFFER
	pDestBuf = LockVideoSurface( guiHelpScreenTextBufferSurface, &uiDestPitchBYTES );
	memset(pDestBuf, 0, HLP_SCRN__HEIGHT_OF_TEXT_BUFFER * uiDestPitchBYTES );
	UnLockVideoSurface( guiHelpScreenTextBufferSurface );
	InvalidateScreen( );
}



// - is up, + is down
void ChangeTopLineInTextBufferByAmount( INT32 iAmouontToMove )
{
	//if we are moving up
	if( iAmouontToMove < 0 )
	{
		if( gHelpScreen.iLineAtTopOfTextBuffer + iAmouontToMove >= 0 )
		{
			//if we can move up by the requested amount
			if( ( gHelpScreen.usTotalNumberOfLinesInBuffer - gHelpScreen.iLineAtTopOfTextBuffer ) > iAmouontToMove )
			{
				gHelpScreen.iLineAtTopOfTextBuffer += iAmouontToMove;
			}

			//else, trying to move past the top
			else
			{
				gHelpScreen.iLineAtTopOfTextBuffer = 0;
			}
		}
		else
		{
			gHelpScreen.iLineAtTopOfTextBuffer = 0;
		}
	}

	//else we are moving down
	else
	{
		//if we dont have to scroll cause there is not enough text
		if( gHelpScreen.usTotalNumberOfLinesInBuffer <= HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER )
		{
			gHelpScreen.iLineAtTopOfTextBuffer = 0;
		}
		else
		{
			if( ( gHelpScreen.iLineAtTopOfTextBuffer + HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER + iAmouontToMove ) <= gHelpScreen.usTotalNumberOfLinesInBuffer )
			{
				gHelpScreen.iLineAtTopOfTextBuffer += iAmouontToMove;
			}
			else
			{
				gHelpScreen.iLineAtTopOfTextBuffer = gHelpScreen.usTotalNumberOfLinesInBuffer - HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER;
			}
		}
	}

//	RenderCurrentHelpScreenTextToBuffer();

	gHelpScreen.ubHelpScreenDirty = HLP_SCRN_DRTY_LVL_REFRESH_TEXT;
}



void DisplayHelpScreenTextBufferScrollBox()
{
	INT32	iSizeOfBox;
	INT32	iTopPosScrollBox=0;
	UINT8	*pDestBuf;
	UINT32 uiDestPitchBYTES;
	UINT16 usPosX;

	if( gHelpScreen.bNumberOfButtons != 0 )
	{
		usPosX = gHelpScreen.usScreenLocX + HLP_SCRN__SCROLL_POSX + HELP_SCREEN_BUTTON_BORDER_WIDTH;
	}
	else
	{
		usPosX = gHelpScreen.usScreenLocX + HLP_SCRN__SCROLL_POSX;
	}

	//
	//first calculate the height of the scroll box
	//

	CalculateHeightAndPositionForHelpScreenScrollBox( &iSizeOfBox, &iTopPosScrollBox );

	//
	// next draw the box
	//

	//if there ARE scroll bars, draw the
	if( !( gHelpScreen.usTotalNumberOfLinesInBuffer <= HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER ) )
	{
		ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX, iTopPosScrollBox, usPosX+HLP_SCRN__WIDTH_OF_SCROLL_AREA,	iTopPosScrollBox+iSizeOfBox-1, Get16BPPColor( FROMRGB( 227, 198, 88 ) ) );

		//display the line
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		// draw the gold highlite line on the top and left
		LineDraw(FALSE, usPosX, iTopPosScrollBox, usPosX+HLP_SCRN__WIDTH_OF_SCROLL_AREA, iTopPosScrollBox, Get16BPPColor( FROMRGB( 235, 222, 171 ) ), pDestBuf);
		LineDraw(FALSE, usPosX, iTopPosScrollBox, usPosX, iTopPosScrollBox+iSizeOfBox-1, Get16BPPColor( FROMRGB( 235, 222, 171 ) ), pDestBuf);

		// draw the shadow line on the bottom and right
		LineDraw(FALSE, usPosX, iTopPosScrollBox+iSizeOfBox-1, usPosX+HLP_SCRN__WIDTH_OF_SCROLL_AREA, iTopPosScrollBox+iSizeOfBox-1, Get16BPPColor( FROMRGB( 65, 49, 6 ) ), pDestBuf);
		LineDraw(FALSE, usPosX+HLP_SCRN__WIDTH_OF_SCROLL_AREA, iTopPosScrollBox, usPosX+HLP_SCRN__WIDTH_OF_SCROLL_AREA, iTopPosScrollBox+iSizeOfBox-1, Get16BPPColor( FROMRGB( 65, 49, 6 ) ), pDestBuf);

		// unlock frame buffer
		UnLockVideoSurface( FRAME_BUFFER );
	}
}


void CreateScrollAreaButtons()
{
	UINT16 usPosX, usWidth, usPosY;
	INT32	iPosY, iHeight;

	if( gHelpScreen.bNumberOfButtons != 0 )
	{
		usPosX = gHelpScreen.usScreenLocX + HLP_SCRN__SCROLL_POSX + HELP_SCREEN_BUTTON_BORDER_WIDTH;
	}
	else
	{
		usPosX = gHelpScreen.usScreenLocX + HLP_SCRN__SCROLL_POSX;
	}

	usWidth = HLP_SCRN__WIDTH_OF_SCROLL_AREA;

	//Get the height and position of the scroll box
	CalculateHeightAndPositionForHelpScreenScrollBox( &iHeight, &iPosY );

		//Create a mouse region 'mask' the entrire screen
	MSYS_DefineRegion( &gHelpScreenScrollArea, usPosX, (UINT16)iPosY, (UINT16)(usPosX+usWidth), (UINT16)(iPosY+HLP_SCRN__HEIGHT_OF_SCROLL_AREA), MSYS_PRIORITY_HIGHEST,
							gHelpScreen.usCursor, SelectHelpScrollAreaMovementCallBack, SelectHelpScrollAreaCallBack );
	MSYS_AddRegion( &gHelpScreenScrollArea );

	guiHelpScreenScrollArrowImage[ 0 ] = LoadButtonImage( "INTERFACE\\HelpScreen.sti", 14,10, 11, 12 ,13 );
	guiHelpScreenScrollArrowImage[ 1 ] = UseLoadedButtonImage( guiHelpScreenScrollArrowImage[ 0 ] ,19,15,16,17,18 );

	if( gHelpScreen.bNumberOfButtons != 0 )
		usPosX = gHelpScreen.usScreenLocX + HLP_SCRN__SCROLL_UP_ARROW_X + HELP_SCREEN_BUTTON_BORDER_WIDTH;
	else
		usPosX = gHelpScreen.usScreenLocX + HLP_SCRN__SCROLL_UP_ARROW_X;

	usPosY = gHelpScreen.usScreenLocY + HLP_SCRN__SCROLL_UP_ARROW_Y;

	//Create the scroll arrows
	giHelpScreenScrollArrows[ 0 ] = QuickCreateButton( guiHelpScreenScrollArrowImage[ 0 ], usPosX, usPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										DEFAULT_MOVE_CALLBACK, BtnHelpScreenScrollArrowsCallback );
	MSYS_SetBtnUserData( giHelpScreenScrollArrows[0],0,0);
	SetButtonCursor( giHelpScreenScrollArrows[0], gHelpScreen.usCursor );

	usPosY = gHelpScreen.usScreenLocY + HLP_SCRN__SCROLL_DWN_ARROW_Y;

	//Create the scroll arrows
	giHelpScreenScrollArrows[ 1 ] = QuickCreateButton( guiHelpScreenScrollArrowImage[ 1 ], usPosX, usPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										DEFAULT_MOVE_CALLBACK, BtnHelpScreenScrollArrowsCallback );
	MSYS_SetBtnUserData( giHelpScreenScrollArrows[1],0,1);
	SetButtonCursor( giHelpScreenScrollArrows[1], gHelpScreen.usCursor );
}


void DeleteScrollArrowButtons()
{
	INT8 i;
	//remove the mouse region that blankets
	MSYS_RemoveRegion( &gHelpScreenScrollArea );

	for( i=0; i<2; i++)
	{
		RemoveButton( giHelpScreenScrollArrows[ i ] );
		UnloadButtonImage( guiHelpScreenScrollArrowImage[ i ] );
	}
}


void CalculateHeightAndPositionForHelpScreenScrollBox( INT32 *piHeightOfScrollBox, INT32 *piTopOfScrollBox )
{
	INT32 iSizeOfBox, iTopPosScrollBox;
	FLOAT dPercentSizeOfBox = 0;
	FLOAT dTemp=0;

	dPercentSizeOfBox = HLP_SCRN__MAX_NUMBER_DISPLAYED_LINES_IN_BUFFER / (FLOAT ) gHelpScreen.usTotalNumberOfLinesInBuffer;

	//if the # is >= 1 then the box is the full size of the scroll area
	if( dPercentSizeOfBox >= 1.0 )
	{
		iSizeOfBox = HLP_SCRN__HEIGHT_OF_SCROLL_AREA;

		//no need to calc the top spot for the box
		iTopPosScrollBox = HLP_SCRN__SCROLL_POSY;
	}
	else
	{
		iSizeOfBox = (INT32)( dPercentSizeOfBox * HLP_SCRN__HEIGHT_OF_SCROLL_AREA + 0.5 );

		//
		//next, calculate the top position of the box
		//
		dTemp = ( HLP_SCRN__HEIGHT_OF_SCROLL_AREA / ( FLOAT ) gHelpScreen.usTotalNumberOfLinesInBuffer ) * gHelpScreen.iLineAtTopOfTextBuffer;

		iTopPosScrollBox = (INT32)( dTemp +.5 ) + HLP_SCRN__SCROLL_POSY;
	}

	if( piHeightOfScrollBox != NULL )
		*piHeightOfScrollBox = iSizeOfBox;

	if( piTopOfScrollBox != NULL )
		*piTopOfScrollBox = iTopPosScrollBox;
}


void SelectHelpScrollAreaCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		gfScrollBoxIsScrolling = FALSE;
		gHelpScreen.iLastMouseClickY = -1;
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		gfScrollBoxIsScrolling = TRUE;
		HelpScreenMouseMoveScrollBox( pRegion->MouseYPos );
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}

}

void SelectHelpScrollAreaMovementCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
//		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
	}
	else if( iReason & MSYS_CALLBACK_REASON_MOVE )
	{
		if( gfLeftButtonState )
		{
			HelpScreenMouseMoveScrollBox( pRegion->MouseYPos );
		}
	}
}

void HelpScreenMouseMoveScrollBox( INT32 usMousePosY )
{
	INT32 iPosY, iHeight;
	INT32 iNumberOfIncrements=0;
	FLOAT dSizeOfIncrement = ( HLP_SCRN__HEIGHT_OF_SCROLL_AREA / ( FLOAT ) gHelpScreen.usTotalNumberOfLinesInBuffer );
	FLOAT dTemp;
	INT32 iNewPosition;

	CalculateHeightAndPositionForHelpScreenScrollBox( &iHeight, &iPosY );

	if( AreWeClickingOnScrollBar( usMousePosY ) || gHelpScreen.iLastMouseClickY != -1 )
	{
		if( gHelpScreen.iLastMouseClickY == -1 )
			gHelpScreen.iLastMouseClickY = usMousePosY;

		if( usMousePosY < gHelpScreen.iLastMouseClickY )
		{
//			iNewPosition = iPosY - ( UINT16)( dSizeOfIncrement + .5);
			iNewPosition = iPosY - ( gHelpScreen.iLastMouseClickY - usMousePosY );

		}
		else if( usMousePosY > gHelpScreen.iLastMouseClickY )
		{
//			iNewPosition = iPosY + ( UINT16)( dSizeOfIncrement + .5);
			iNewPosition = iPosY + usMousePosY - gHelpScreen.iLastMouseClickY;
		}
		else
		{
			return;
		}

		dTemp = ( iNewPosition - iPosY ) / dSizeOfIncrement;

		if( dTemp < 0 )
			iNumberOfIncrements = (INT32)( dTemp - 0.5 );
		else
			iNumberOfIncrements = (INT32)( dTemp + 0.5 );

		gHelpScreen.iLastMouseClickY = usMousePosY;

//		return;
	}
	else
	{
		//if the mouse is higher then the top of the scroll area, set it to the top of the scroll area
		if( usMousePosY < HLP_SCRN__SCROLL_POSY )
			usMousePosY = HLP_SCRN__SCROLL_POSY;

		dTemp = ( usMousePosY - iPosY ) / dSizeOfIncrement;

		if( dTemp < 0 )
			iNumberOfIncrements = (INT32)( dTemp - 0.5 );
		else
			iNumberOfIncrements = (INT32)( dTemp + 0.5 );
	}

	//if there has been a change
	if( iNumberOfIncrements != 0 )
	{
		ChangeTopLineInTextBufferByAmount( iNumberOfIncrements );
	}
}



void BtnHelpScreenScrollArrowsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		INT32 iButtonID = MSYS_GetBtnUserData( btn, 0);

		btn->uiFlags |= BUTTON_CLICKED_ON;

		//if up
		if( iButtonID == 0 )
		{
			ChangeTopLineInTextBufferByAmount( -1 );
		}
		else
		{
			ChangeTopLineInTextBufferByAmount( 1 );
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		INT32 iButtonID = MSYS_GetBtnUserData( btn, 0);

		//if up
		if( iButtonID == 0 )
		{
			ChangeTopLineInTextBufferByAmount( -1 );
		}
		else
		{
			ChangeTopLineInTextBufferByAmount( 1 );
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}

}


BOOLEAN AreWeClickingOnScrollBar( INT32 usMousePosY )
{
	INT32 iPosY, iHeight;

	CalculateHeightAndPositionForHelpScreenScrollBox( &iHeight, &iPosY );

	if( usMousePosY >= iPosY && usMousePosY < ( iPosY + iHeight ) )
		return( TRUE );
	else
		return( FALSE );
}

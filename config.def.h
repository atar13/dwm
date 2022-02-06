/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const int startwithgaps[]    = { 1 };	/* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]   = { 10 };   /* default gap between windows in pixels, this can be customized for each tag */
static const unsigned int snap      = 32;       /* snap pixel */
static const int scalepreview       = 4;        /* tag preview scaling */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 0;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */

#define ICONSIZE (bh - 3)   /* icon size */
#define ICONSPACING 5 /* space between icon and title */

static const char *fonts[]          = {"JetBrainsMono Nerd Font", "FiraCode Nerd Font", "MesloLGS NF", "Noto Color Emoji","NotoColorEmoji","Hack:size=10"};
static const char dmenufont[]       = "MesloLGS NF:size=10";
static const char col_gray1[]       = "#2D3132";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_purple[]      = "#9400D3";
static const char col_black[] 	    = "#000000";
static const char col_gray[]	    = "#545454";
static const char col_brightpurple[] = "#B968C7";
static const char col_pastelpurple[] = "#A599E9";
static const char col_clear[] 	    = "#a9a9a9";
static const char col_pink[] 	    = "#F58FE9";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_black, col_gray2 },
	[SchemeSel]  = { col_black, col_brightpurple,  col_brightpurple },
};

static const char *const autostart[] = {
  "go-dwm-statusbar", NULL,
	"sh", "-c", "/home/atarbinian/.fehbg", NULL,
	"picom", "--experimental-backends", NULL,
	"dunst", NULL,
	"nm-applet", NULL,
	"pnmixer", NULL,
	"blueman-applet", NULL,
	"systemctl", "start", "--user", "ulauncher.service", NULL,
	"/home/atarbinian/.local/bin/locker", NULL,
	"lxpolkit", "NULL",
	"sh","-c", "\"/home/atarbinian/.dwm/gnome-keyring.sh\"", NULL,
	NULL /* terminate */
};

/* tagging */
#define MAX_TAGNAME_LEN 14		/* excludes TAG_PREPEND */
#define TAG_PREPEND "%s  "		/* formatted as 2 chars */
#define MAX_TAGLEN 16			/* altogether */
static char tags[][MAX_TAGLEN] = { "", "", "", "", "", "", "", "", "" };
static char const const_tags[][MAX_TAGLEN] = { "", "", "", "", "", "", "", "", "" };


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Ulauncher Preferences",  NULL,       NULL,       0,       1,           -1 },
	{ "Zoom",  NULL,       NULL,       0,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

/* mouse scroll resize */
static const int scrollsensetivity = 30; /* 1 means resize window by 1 pixel for each scroll event */

#include "horizgrid.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "<F>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ "#G#",      horizgrid },
	// { ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_gray3, "-sb", col_purple, "-sf", col_gray4, "-h", "30", NULL };
static const char *roficmd[] = {"rofi", "-no-lazy-grab", "-show", "drun", "-modi", "drun", "-theme", "~/.config/rofi/launchers/colorful/style_7.rasi", NULL };

static const char *termcmd[]  = { "kitty", NULL };

static const char *lockcmd[] = { "/home/atarbinian/.local/bin/lock", "&", NULL };
static const char *firefoxcmd[] = {"firefox", NULL };
static const char *firefoxprivatecmd[] = {"firefox", "--private-window", NULL };

/*brightness comamnds */
static const char *inc_brightness[] = {"light", "-A", "5", NULL};
static const char *dec_brightness[] = {"light", "-U", "5", NULL};

/*volume commands */
static const char *toggle_Mute[] = {"pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL};
static const char *inc_volume[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL};
static const char *dec_volume[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL};

static const char *play_cmd[] = {"playerctl", "play-pause", NULL};
static const char *next_cmd[] = {"playerctl", "next", NULL};
static const char *previous_cmd[] = {"playerctl", "previous", NULL};

static const char *scrotcmd[] = {"sh", "/home/atarbinian/.scrot-select.sh", NULL};

#include "shift-tools.c"

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
  /* { MODKEY,                       XK_space,  spawn,      	   {.v = roficmd} }, */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = firefoxcmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = firefoxprivatecmd } },
	/* { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd } }, */
	{ MODKEY|ShiftMask,             XK_v,      spawn,          {.v = scrotcmd } },
	{ 0,                            XK_Print,      						 spawn,          {.v = scrotcmd } },
	{ 0,             								XF86XK_MonBrightnessDown,  spawn,          {.v = dec_brightness } },
	{ 0,           									XF86XK_MonBrightnessUp,    spawn,          {.v = inc_brightness } },
	{ 0,             								XF86XK_AudioMute,     		 spawn,          {.v = toggle_Mute } },
	{ 0,             								XF86XK_AudioLowerVolume,   spawn,          {.v = dec_volume } },
	{ 0,             								XF86XK_AudioRaiseVolume,   spawn,          {.v = inc_volume } },
	{ 0,             								XF86XK_AudioPlay,     		 spawn,          {.v = play_cmd } },
	{ 0,             								XF86XK_AudioPrev,   			 spawn,          {.v = previous_cmd } },
	{ 0,             								XF86XK_AudioNext,     		 spawn,          {.v = next_cmd } },
	{ MODKEY|ShiftMask,             XK_n,      nametag,        {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_a,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_s,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_apostrophe,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_semicolon,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,	                			XK_q, shiftview,    { .i = -1 } },
	{ MODKEY,                       XK_w, shiftview,    { .i = +1 } },
	{ MODKEY|ShiftMask,							XK_q,      shiftboth,      { .i = -1 }	},
	{ MODKEY|ShiftMask,             XK_w,      shiftboth,      { .i = +1 }	},
	{ MODKEY|ControlMask,						XK_q,      shiftswaptags,  { .i = -1 }	},
	{ MODKEY|ControlMask,						XK_w,      shiftswaptags,  { .i = +1 }	},
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ ALT,                       		XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	// { MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	// { MODKEY,                       XK_Tab,  setlayout,      {0} },
	{ MODKEY,             			XK_Tab,      layoutscroll,   {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Tab,      layoutscroll,   {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,             					XK_bracketleft, 		setborderpx,    {.i = -1 } },
	{ MODKEY,             					XK_bracketright, 		setborderpx,    {.i = +1 } },
	{ MODKEY,             					XK_backslash, 	setborderpx,    {.i = 0 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_grave,      quit,           {0} },
};

/* resizemousescroll direction argument list */
static const int scrollargs[][2] = {
	/* width change         height change */
	{ +scrollsensetivity,	0 },
	{ -scrollsensetivity,	0 },
	{ 0, 				  	+scrollsensetivity },
	{ 0, 					-scrollsensetivity },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY,         Button4,        resizemousescroll, {.v = &scrollargs[0]} },
	{ ClkClientWin,         MODKEY,         Button5,        resizemousescroll, {.v = &scrollargs[1]} },
	{ ClkClientWin,         MODKEY,         Button6,        resizemousescroll, {.v = &scrollargs[2]} },
	{ ClkClientWin,         MODKEY,         Button7,        resizemousescroll, {.v = &scrollargs[3]} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

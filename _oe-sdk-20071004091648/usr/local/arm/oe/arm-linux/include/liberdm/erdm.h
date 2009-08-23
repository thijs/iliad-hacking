#ifndef ERDM_H
#define ERDM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// #define DM_FULL_UPDATE 0

/*
 *  Priority of a display update.
 *  The display manager waits a short time for further display update commands
 *  and then combines all commands received into a single display update.
 */
typedef enum
{
    dmCmdPriorLow = 0,  // wait 1500 ms
    dmCmdPriorNormal,   // wait  500 ms
    dmCmdPriorHigh,     // wait  100 ms
    dmCmdPriorUrgent    // no wait, immediate display update
} eDmCmdPriority;

/*
 *  Waveform to be used for display update.
 */
typedef enum
{
    dmQFull = 0,        // fullscreen waveform - makes display flicker once, sixteen gray levels
    dmQBW,              // black-and-white waveform - fastest one but horrible for grey pixels
    dmQTyping,          // typing waveform - looks nice, no display flicker, only four gray levels (0, 5, 10, 15)
    dmQUndefined
} eDmQuality;

/*
 *  Command for display manager (server)
 *  These must not be used directly, better pass through functions below
 */
typedef enum
{
    dmCcDisplay = 0,      // do a full display update
    dmCcDisplayPartial,   // not implemented
    dmCcDump,             // not implemented
    dmCcEraseToWhite,     // blank display
    dmCcUndefined
} eDmCommandCode;


/*
 *  Functions to send a command to the display manager.
 */

// Do a full display update
int     dmDisplay(eDmCmdPriority prior, eDmQuality qual);

// Blank display
int     dmDisplayEraseToWhite(eDmCmdPriority prior);

// For system use only
int     dmMessageInit(char *szDestination);

// Not implemented
int     dmDisplayPartial(eDmCmdPriority prior, eDmQuality qual, int xUp, int yUp, int xDown, int yDown );
int     dmDump(eDmCmdPriority prior);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

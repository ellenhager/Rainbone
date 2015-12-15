/*******************************************************************************
 *
 * Util file for stuff that needs to accessed globaly
 *
 ******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3


#ifndef SOUNDFILE
#define SOUNDFILE
// If a new music file is added, be sure to add a enum for it here as well.
enum SoundFile { INTROMUSIC, EVILMUSIC, CATAAHH, CATHELP, ELOCK, SLOCK, WIN };
#endif // SOUNDFILE

#ifndef WORD
#define WORD
// Enum for our 3D words
enum Word { ONE, TWO, THREE, FOUR, FIVE, START, LAST};
#endif // WORD
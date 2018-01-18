// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <shellapi.h>
#include "NotificationHelper.h"
#include <stdexcept>
#include <commctrl.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <algorithm>
#include "Note.h"
#include "NotesHolder.h"
#include "PiePart.h"
#include <iomanip>
#include <math.h>
#include <locale>
#include <codecvt>
// TODO: reference additional definitions your program requires here

#define CONTENTS_FILE_NAME "contents"
#define TAGS_FILE_NAME "tags"
#define TITLES_FILE_NAME "titles"
#define DATA_EXTENSION_NAME ".dat"
#define MAX_LINE_BUFFER 1024
#define MAX_CONTENT_BUFFER 8192
#define FILE_PATH ""
#define MAX_PREVIEW_BUFFER 50
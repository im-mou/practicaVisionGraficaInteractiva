#ifndef  __FG_STRUCT_H__
#define  __FG_STRUCT_H__

/*
 * fg_struct.h
 *
 * The GLUT-compatible part of the freeglut library include file
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Thu Dec 2 1999
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* -- FONT STRUCTURES -------------------------------------------------- */

/* The bitmap font structure */
	typedef struct tagSFG_Font SFG_Font;
	struct tagSFG_Font
	{
		char* Name;         /* The source font name             */
		int             Quantity;     /* Number of chars in font          */
		int             Height;       /* Height of the characters         */
		const GLubyte** Characters;   /* The characters mapping           */

		float           xorig, yorig; /* Relative origin of the character */
	};

/* The stroke font structures */

	typedef struct tagSFG_StrokeVertex SFG_StrokeVertex;
	struct tagSFG_StrokeVertex
	{	GLfloat         X, Y;
	};

	typedef struct tagSFG_StrokeStrip SFG_StrokeStrip;
	struct tagSFG_StrokeStrip
	{
		int             Number;
		const SFG_StrokeVertex* Vertices;
	};

	typedef struct tagSFG_StrokeChar SFG_StrokeChar;
	struct tagSFG_StrokeChar
	{
		GLfloat         Right;
		int             Number;
		const SFG_StrokeStrip* Strips;
	};

	typedef struct tagSFG_StrokeFont SFG_StrokeFont;
	struct tagSFG_StrokeFont
	{
		char* Name;                       /* The source font name      */
		int             Quantity;                   /* Number of chars in font   */
		GLfloat         Height;                     /* Height of the characters  */
		const SFG_StrokeChar** Characters;          /* The characters mapping    */
	};

/* A helper structure holding two ints and a boolean */
	typedef struct tagSFG_XYUse SFG_XYUse;
	struct tagSFG_XYUse
	{
		GLint           X, Y;               /* The two integers...               */
		GLboolean       Use;                /* ...and a single boolean.          */
	};

/* A list structure */
	typedef struct tagSFG_List SFG_List;
	struct tagSFG_List
	{
		void* First;
		void* Last;
	};

/* This structure holds different freeglut settings */
	typedef struct tagSFG_State SFG_State;
	struct tagSFG_State
	{
		SFG_XYUse        Position;             /* The default windows' position  */
		SFG_XYUse        Size;                 /* The default windows' size      */
		unsigned int     DisplayMode;          /* Display mode for new windows   */

		GLboolean        Initialised;          /* freeglut has been initialised  */

		int              DirectContext;        /* Direct rendering state         */

		GLboolean        SkipStaleMotion;      /* skip stale motion events */

		GLboolean        StrokeFontDrawJoinDots;/* Draw dots between line segments of stroke fonts? */

	};
/*** END OF FILE ***/

#endif /* __FG_STRUCT_H__ */


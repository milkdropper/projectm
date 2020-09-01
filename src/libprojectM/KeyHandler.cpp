/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2004 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */

#include <stdio.h>

#include "Common.hpp"
#include "fatal.h"
#include "KeyHandler.hpp"
#include "event.h"
#include "BeatDetect.hpp"
#include "PresetChooser.hpp"
#include "Renderer.hpp"
#include "projectM.hpp"

#include <iostream>
#include "TimeKeeper.hpp"


class Preset;
interface_t current_interface = DEFAULT_INTERFACE;

void selectRandom(const bool hardCut);
void selectNext(const bool hardCut);
void selectPrevious(const bool hardCut);

std::string round_float(float number)
{
    std::string num_text = std::to_string(number);
    std::string rounded = num_text.substr(0, num_text.find(".")+3);
	return rounded;
}

void refreshConsole() {

  switch (current_interface) {

  case MENU_INTERFACE:
    // unimplemented
    break;
  case SHELL_INTERFACE:
    // unimplemented
    break;
  case EDITOR_INTERFACE:
    // unimplemented
    break;
  case DEFAULT_INTERFACE:
    break;
  case BROWSER_INTERFACE:
    // unimplemented
    break;
  default:
    break;
  }

}

void projectM::key_handler( projectMEvent event,
                            projectMKeycode keycode, projectMModifier modifier ) {

	switch( event ) {


	case PROJECTM_KEYDOWN:

	  //default_key_handler();
	  switch (current_interface)
	    {

	    case MENU_INTERFACE:
//	      menu_key_handler(this, event, keycode);
	      break;
	    case SHELL_INTERFACE:
	      //shell_key_handler();
	      break;
	    case EDITOR_INTERFACE:
//	      editor_key_handler(event,keycode);
	      break;
	    case BROWSER_INTERFACE:
//	      browser_key_handler(event,keycode,modifier);
	      break;
	    case DEFAULT_INTERFACE:
	      default_key_handler(event,keycode);
	      break;
	    default:
	      default_key_handler(event,keycode);
	      break;

	    }
	  break;
	default:
		break;

	}
}

void projectM::default_key_handler( projectMEvent event, projectMKeycode keycode) {

	switch( event ) {

	case PROJECTM_KEYDOWN:

		switch (keycode)
		{
		case PROJECTM_K_HOME:
			if (renderer->showmenu) { // pageup only does something when the preset menu is active.
				selectPreset(0);  // jump to top of presets.
			}
			break;
		case PROJECTM_K_END:
			if (renderer->showmenu) { // pageup only does something when the preset menu is active.
				selectPreset(m_presetLoader->size() - 1);  // jump to bottom of presets.
			}
			break;
		case PROJECTM_K_PAGEUP:
			if (renderer->showmenu) { // pageup only does something when the preset menu is active.
				int upPreset = m_presetPos->lastIndex() - (renderer->textMenuPageSize / 2.0f); // jump up by page size / 2
				if (upPreset < 0) // handle lower boundary
					upPreset = m_presetLoader->size() - 1;
				selectPreset(upPreset);  // jump up menu half a page.
			}
			break;
		case PROJECTM_K_PAGEDOWN:
			if (renderer->showmenu) { // pagedown only does something when the preset menu is active.
				int downPreset = m_presetPos->lastIndex() + (renderer->textMenuPageSize / 2.0f); // jump down by page size / 2
				if (downPreset >= (m_presetLoader->size() - 1)) // handle upper boundary
					downPreset = 0;
				selectPreset(downPreset); // jump down menu half a page.
			}
			break;
		case PROJECTM_K_UP:
			if (renderer->showmenu) {
				selectPrevious(true);
			}
			else {
				beatDetect->beatSensitivity += 0.01;
				if (beatDetect->beatSensitivity > 5.0) beatDetect->beatSensitivity = 5.0;
				renderer->setToastMessage("Beat Sensitivity: " + round_float(beatDetect->beatSensitivity));
			}
			break;
		case PROJECTM_K_DOWN:
			if (renderer->showmenu) {
				selectNext(true);
			}
			else {
				beatDetect->beatSensitivity -= 0.01;
				if (beatDetect->beatSensitivity < 0) beatDetect->beatSensitivity = 0;
				renderer->setToastMessage("Beat Sensitivity: " + round_float(beatDetect->beatSensitivity));
			}
			break;
		case PROJECTM_K_h:
			renderer->showhelp = !renderer->showhelp;
			renderer->showstats = false;
			renderer->showmenu = false;
		case PROJECTM_K_F1:
			renderer->showhelp = !renderer->showhelp;
			renderer->showstats = false;
			renderer->showmenu = false;
			break;
		case PROJECTM_K_y:
			this->setShuffleEnabled(!this->isShuffleEnabled());
			if (this->isShuffleEnabled()) {
				renderer->setToastMessage("Shuffle Enabled");
			}
			else {
				renderer->setToastMessage("Shuffle Disabled");
			}
			break;

		case PROJECTM_K_F5:
			renderer->showfps = !renderer->showfps;
			// Initialize counters and reset frame count.
			renderer->lastTimeFPS = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			renderer->currentTimeFPS = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			renderer->totalframes = 0;
			// Hide preset name from screen and replace it with FPS counter.
			if (renderer->showfps)
			{
				renderer->showpreset = false;
			}
			break;
		case PROJECTM_K_F4:
			renderer->showstats = !renderer->showstats;
			if (renderer->showstats) {
				renderer->showhelp = false;
				renderer->showmenu = false;
			}
			break;
		case PROJECTM_K_F3: {
			renderer->showpreset = !renderer->showpreset;
			// Hide FPS from screen and replace it with preset name.
			if (renderer->showpreset)
			{
				renderer->showfps = false;
			}
			break;
		}
		case PROJECTM_K_F2:
			renderer->showtitle = !renderer->showtitle;
			break;
#ifndef MACOS
		case PROJECTM_K_F9:
#else
		case PROJECTM_K_F8:
#endif

			renderer->studio = !renderer->studio;
			break;

		case PROJECTM_K_ESCAPE: {
			//	        exit( 1 );
			break;
		}
		case PROJECTM_K_f:

			break;
		case PROJECTM_K_a:
			renderer->correction = !renderer->correction;
			break;
		case PROJECTM_K_b:
			break;
		case PROJECTM_K_H:
		case PROJECTM_K_m:
			renderer->showmenu = !renderer->showmenu;
			if (renderer->showmenu) {
				renderer->showhelp = false;
				renderer->showstats = false;
				populatePresetMenu();
			}
			break;
		case PROJECTM_K_M:
			renderer->showmenu = !renderer->showmenu;
			if (renderer->showmenu)
			{
				renderer->showhelp=false;
				renderer->showstats=false;
				populatePresetMenu();
			}
          break;
      case PROJECTM_K_n:
          selectNext(true);
          break;
      case PROJECTM_K_N:
          selectNext(false);
          break;
	    case PROJECTM_K_r:
		selectRandom(true);
		break;
	    case PROJECTM_K_R:
		selectRandom(false);
		break;
	    case PROJECTM_K_p:
	      selectPrevious(true);
	      break;
	    case PROJECTM_K_P:
	    case PROJECTM_K_BACKSPACE:
	      selectPrevious(false);
	      break;
	    case PROJECTM_K_l:
			setPresetLock(!isPresetLocked());
			break;
	    case PROJECTM_K_s:
            	renderer->studio = !renderer->studio;
	    case PROJECTM_K_i:
	        break;
	    case PROJECTM_K_z:
	      break;
	    case PROJECTM_K_0:
//	      nWaveMode=0;
	      break;
	    case PROJECTM_K_6:
//	      nWaveMode=6;
	      break;
	    case PROJECTM_K_7:
//	      nWaveMode=7;
	      break;
	    case PROJECTM_K_t:
	      break;
	    case PROJECTM_K_EQUALS:
	    case PROJECTM_K_PLUS:

	    	unsigned int index;

	    	if (selectedPresetIndex(index)) {

	    		const int oldRating = getPresetRating(index, HARD_CUT_RATING_TYPE);

	    		if (oldRating >= 6)
	    			  break;

	    		const int rating = oldRating + 1;

	    		changePresetRating(index, rating, HARD_CUT_RATING_TYPE);
	    	}

	    	break;

	    case PROJECTM_K_MINUS:
	    	if (selectedPresetIndex(index)) {

	    		const int oldRating = getPresetRating(index, HARD_CUT_RATING_TYPE);

	    		if (oldRating <= 1)
	    			  break;

	    		const int rating = oldRating - 1;

	    		changePresetRating(index, rating, HARD_CUT_RATING_TYPE);
	    	}
	    	break;

	    default:
	      break;
	    }
	default:
		break;

	}
}

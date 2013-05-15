/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef MAINAPP_H_
#define MAINAPP_H_

/* commented by Cristian.Guef
#include <nlib/log.h>
*/
//added by Cristian.Guef
#include "Log.h"

#include <nlib/datetime.h>


#include "ConfigApp.h"

#include "hostapp/CommandsThread.h"

namespace nisa100 {

class MainApp
{
public:
	/* commented by Cristian.Guef
	LOG_DEF("nisa100.MainApp");
	*/

public:
	
	int Run(int argc, char* argv[]);

	void LoadConfig(bool reloadConfiguration);
	
	void ReloadConfig();
	void RunStep(hostapp::CommandsThread& p_rCommandsThread);
	
private:
	ConfigApp config;
};


} //namesapce nisa100

#endif /*MAINAPP_H_*/

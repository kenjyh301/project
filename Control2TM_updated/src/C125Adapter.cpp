//============================================================================
// Name        : C125Adapter.cpp
// Author      : hungnv103
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "C125Adapter.h"

int main() {

	delete_queue();
	SystemConfigurations::getInstance()->init(CONFIGURE_FILE);
	C125DataProcessor processor;
	MonitorProcessor c125Monitor;

	setMutex(processor, c125Monitor);

	processor.run();
	c125Monitor.run();
	while(1){
		sleep(100);
	}
	return 0;
}

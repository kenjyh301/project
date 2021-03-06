/*
 * SystemConfigurations.h
 *
 *  Created on: Sep 23, 2017
 *      Author: hungnv103
 */

#ifndef SYSTEMCONFIGURATIONS_H_
#define SYSTEMCONFIGURATIONS_H_

#include <string>
#include <iostream>

class SystemConfigurations {
public:
	void init(std::string configFilePath);
	virtual ~SystemConfigurations();

	static SystemConfigurations *getInstance();
    std::string getHostIp() const;

//    void setHostIp(std::string mHostIp);

    std::string getPort() const;

    void setPort(std::string mPort);
//	std::string GatewayIp;
	std::string GatewayPort;

private:
	SystemConfigurations();

private:
//	std::string mHostIp;
	std::string mPort;

};

#endif /* SYSTEMCONFIGURATIONS_H_ */

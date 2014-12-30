/*******************************
Copyright (C) 2013-2015 gregoire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/

#include "SystemMonitor.h"
#include <QMessageBox>

SystemMonitor *SystemMonitor::monitor = new SystemMonitor();

SystemMonitor::SystemMonitor() : QThread(), updateTime(0.5), systemInfos(retrieveSystemInfos()) {
	if(!enableDebugPrivileges(true)) {
		QMessageBox::warning(0, QObject::tr("Unable to get debug privileges."), QObject::tr("Unable to get debug privileges, did you start with administrator rights ?"));
	}
	start();
}

SystemMonitor::~SystemMonitor() {
}

double SystemMonitor::getMemoryUsage() const {
	return perfInfos.mem;
}

double  SystemMonitor::getCpuUsage(int core) const {
	return core < 0 ? perfInfos.cpuTotal : perfInfos.cpuCores[core];
}

ullong SystemMonitor::getTotalMemory() const {
	return systemInfos ? systemInfos->totalMemory : 0;
}

uint SystemMonitor::getCpuCount() const {
	return systemInfos ? systemInfos->cpus : 0;
}

SystemInfo *SystemMonitor::retrieveSystemInfos() {
	if(systemInfos) {
		return systemInfos;
	}
	systemInfos = getSystemInfo();
	if(systemInfos && systemInfos->cpus > 0) {
		perfInfos.cpuCores = new double[systemInfos->cpus];
		counters.cpuCores = new CpuPerfCounter*[systemInfos->cpus];
		for(uint i = 0; i != systemInfos->cpus; i++) {
			counters.cpuCores[i] = new CpuPerfCounter(i);
			perfInfos.cpuCores[i] = 0;
		}
	}
	return systemInfos;
}

void SystemMonitor::run() {
	qDebug("Monitor started");
	while(true) {
		if(retrieveSystemInfos()) {
			perfInfos.cpuTotal = counters.cpuTotal / 100;
			perfInfos.mem = counters.mem / systemInfos->totalMemory;
			for(uint i = 0; i != systemInfos->cpus; i++) {
				perfInfos.cpuCores[i] = *counters.cpuCores[i];
			}
			emit(infoUpdated());
		}
		msleep(updateTime * 1000);
	}
}

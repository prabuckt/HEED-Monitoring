int seq = 0;
Data* readings = new Data();



void resetReadings(Data* readings){
  readings->unixtime=0;
  readings->solarBatt=0;
  readings->usage=0;
  readings->charging=0;
  readings->interrupt=0;
  readings->inactivity=0;
  readings->activity=0;
  readings->nodeBatt=0;
  readings->seq=0;
}

/**
 * The function takes readings from the battery sensors, and checks the ADXL345 for
 * an interrupt. The data is then checked if it is eventful, and if so is stored 
 * to an SD card.
 */
void doSenseCycle()
{
  getTime(readings);
  getSolarBatteryVoltage(readings);
  getBatteryVoltage(readings);
  adxl345GetInterrupt(readings);
  getLanternState(readings);
  
  if(hasEvent(readings))
  {
    readings->seq = seq; 
    String pkt = constructPkt(readings);
    
    connectGSM();
    connectMQTT();

    bool transmit_res = transmit(MQTT_TOPIC, pkt);
    bool csvWriteRes = writeDataToFile(readings);
    
    disconnectMQTT();
    disconnectGSM();
    if (transmit_res || csvWriteRes) { 
      updateState(readings); 
    }
    else {} //Do something for failure
    resetReadings(readings);
    seq++; //increment sequence number*/
    }
}

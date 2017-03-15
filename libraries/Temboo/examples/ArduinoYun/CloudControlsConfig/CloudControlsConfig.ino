// Copyright 2016, Temboo Inc.

#include <Process.h>

void setup() {
  // initialize the Bridge
  Bridge.begin();
  Serial.begin(9600);
  Process p;

  //intro message 
  Serial.println("**** Temboo Cloud Controls ****\n");
  
  // update the package list
  Serial.print("Updating package listings...");
  p.runShellCommand("opkg update");
  int returnCode = p.exitValue();
  if (returnCode == 0) {
    Serial.println("Success!");
  } else {
    Serial.println("Failed. Make sure your device is connected to the internet properly.");
    while(p.available()) {
      char c = p.read();
      Serial.print(c);
    }
    return;
  }
  Serial.println();
  // upgrade the Temboo package
  Serial.print("Updating Temboo...");
  p.runShellCommand("opkg install http://downloads.arduino.cc/openwrtyun/1/packages/temboo_1.4.0-1_ar71xx.ipk");
  returnCode = p.exitValue();
  if (returnCode == 0) {
    Serial.println("Success!");
  } else {
    Serial.println("Failed.");
    while(p.available()) {
      char c = p.read();
      Serial.print(c);
    }
    return;
  }
  Serial.println();
  
  // install python openssl to allow for for ssl connections
  Serial.print("Installing python-openssl...");
  p.runShellCommand("opkg install python-openssl");
  returnCode = p.exitValue();
  if (returnCode == 0) {
    Serial.println("Success!");
  } else {
    Serial.println("Failed.");
    while(p.available()) {
      char c = p.read();
      Serial.print(c);
    }
    return;
  }
  Serial.println();

  // Installing twisted web to work with CoAP gateway
  Serial.print("Installing twisted-web...");
  p.runShellCommand("opkg install twisted-web");
  returnCode = p.exitValue();
  if (returnCode == 0) {
    Serial.println("Success!");
  } else {
    Serial.println("Failed.");
    while(p.available()) {
      char c = p.read();
      Serial.print(c);
    }
    return;
  }
  Serial.println();

  // Configuring zope
  Serial.print("Configuring zope...");
  p.runShellCommand("touch /usr/lib/python2.7/site-packages/zope/__init__.py");
  returnCode = p.exitValue();
  if (returnCode == 0) {
    Serial.println("Success!");
  } else {
    Serial.println("Failed.");
    while(p.available()) {
      char c = p.read();
      Serial.print(c);
    }
    return;
  }
  Serial.println();
  Serial.println("Update Complete - your Yun is ready for Cloud Controls!");
}

void loop() {
  // do nothing
}

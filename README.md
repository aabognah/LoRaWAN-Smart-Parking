# LoRa enabled Smart Parking Solution with loriot.io, PubNub, and IBM Bluemix

The implementation of this solution was based in part on Shympurk’s smart parking solution on gitbu.com : https://github.com/shyampurk/bluemix-parking-meter

This is a LoRaWAN enabled smart parking solution that allows real-time detection of the status of parking spots through an Arduino based sensors that send data via a LoRaWAN network to a mobile app connected to an application running on IBM Bluemix.


## OVERVIEW

The LoRaWAN smart-parking solution used an Arduino based sensor node, a multi-tech LoRa Gateway, a Loriot.io LoRa Server, a Pubnub Server, and an IBM Bluemix to host the Parking application server. The following are the detailed steps of how the application works:

1-	The Arduino board detects the status of the parking spot by using an HC-SR04 ultrasonic sensors. 
2-	The status data is sent to the Multi-Tech LoRa gateway via a Multi-Tech mDot LoRa module connected to the Arduino board via serial connection.
3-	The LoRa gateway is connected to a LoRa server on Loriot.io and forwards all packets it receives there
4-	The Loriot server forwards the received packages to a PubNub channel that the Application server is listening on
5-	The Application Server hosted on IBM Bluemix listens to the PubNub channel and detects the status data of each parking spot.
6-	After comparing the parking spot status data with the stored value, the parking application makes the decision of whether or not to send a notice message to the mobile app to update the status of a parking spot


## BUILD AND INSTALL

Refer [Build & Install](BUILD.md) Steps

## APPLICATION SERVER HOSTING

For hosting the Parking Management Server on IBM Bluemix, follow the steps as given below


1. Signup to create your trial [IBM Bluemix account](https://developer.ibm.com/bluemix/#gettingstarted).
 
3. Follow the [Bluemix Documentation](https://www.ng.bluemix.net/docs/) to create your bluemix container 

4. Install the [PubNub service](https://www.pubnub.com/blog/2015-09-09-getting-started-pubnub-ibm-bluemix/) and attach it to your container.
 
5. Install and initialize the buildpacks for [python application runtime](https://www.ng.bluemix.net/docs/starters/python/index.html) 

6. Deploy the application server package under ""parking-meter" folder, on your Bluemix container and start the application from Bluemix console.



## WORKING

1) Deploy the Parking management Server application on IBM Bluemix

2) Install the Mobile app in an android phone and ensure that the phone has access to internet.

3) Make sure that the ultrasonic sensors are not obstructed 

4) Launch the mobile app. Upon launching, it will ask the user for entering his vehicle registration ( license plate ) number. Feed in the number and proceed. 

5) The app will now display the parking map with color coded status indicators for all parking spaces. Check that the initial status of all the parking spaces is green, which indicates that all parking spaces are free.

6) Obstruct one or more ultrasonic sensors with an object to simulate the presence of a vehicle. 

7) Observe the display of mobile app. After a few seconds the color for the corrosponding slot numbers should turn red to indicate that the space have been occupied. If the mobile app indicates grey status for any of the parking slot then this means that the sensor has a fault or is malfunctioning and the space is currently unavailable for parking. 

8) For reserving a parking space, tap on a vacant (green) parking slot on the mobile app. The app will display a message indicating that the parking space has been reserved for the user. Also the color of the slot, on the map, will change to red.

9) Subsequently, obstruct the corrosponding sensor to simulate the parking of car. If this is not done within one minute then the app will assume that user reserved the parking space but never parked and hence it will cancel the reservation and charge a minimum bill. 

10) After sometime , remove the obstruction from the ultrasonic sensor to simulate pulling out of the car from the parking space. This indicates end of praking time.

11) The mobile app will automatically detect end of parking and display a message to user providing the details of his parking session along with the outstanding bill amount.




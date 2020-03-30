from pyngrok import ngrok
import paho.mqtt.client as mqtt
import time
mqttc=mqtt.Client()
mqttc.connect("test.mosquitto.org", 1883, 60)                                  # Mqtt broker
ngrok.set_auth_token("1YP3idsdLxEmQ4K0BFZtCPWkDTx_2N1PEJuDtG1Japmxa4biP")      # Enter the registered Auth_Token
public_url = ngrok.connect("http://192.168.1.14:81")                       # tunnel to host:port instead of localhost

print(public_url)                                                              # Displaying the ngrok_tunnel url 

while True:
    time.sleep(1)
    mqttc.publish("ngrok_test",public_url)                                     
    mqttc.loop(2)
    time.sleep(120) 

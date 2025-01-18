#include "WiFiProv.h"
#include "WiFi.h"

const char *pop = "abcd1234";           
const char *service_name = "PROV_123";           
bool reset_provisioned = false;   


void SysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("\nConnected! IP address: ");
      Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("\nDisconnected. Reconnecting to the AP...");
      break;

    case ARDUINO_EVENT_PROV_START:
      Serial.println("\nProvisioning started. Provide credentials using the smartphone app.");
      break;

    case ARDUINO_EVENT_PROV_CRED_RECV:
      Serial.println("\nReceived Wi-Fi credentials");
      Serial.print("\tSSID: ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.ssid);
      Serial.print("\tPassword: ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.password);
      break;

    case ARDUINO_EVENT_PROV_CRED_FAIL:
      Serial.println("\nProvisioning failed!\nPlease reset and retry.");
      break;

    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("\nProvisioning successful!");
      break;

    case ARDUINO_EVENT_PROV_END:
      Serial.println("\nProvisioning ended.");
      break;

    default:
      Serial.println("\nUnhandled event.");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.onEvent(SysProvEvent);

#if CONFIG_BLUEDROID_ENABLED && !defined(USE_SOFT_AP)
  Serial.println("Begin Provisioning using BLE");
  uint8_t uuid[16] = {0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf, 0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02};
  WiFiProv.beginProvision(NETWORK_PROV_SCHEME_BLE, NETWORK_PROV_SCHEME_HANDLER_FREE_BLE, NETWORK_PROV_SECURITY_1, pop, service_name, NULL, uuid, reset_provisioned);
  log_d("BLE QR Code:");
  WiFiProv.printQR(service_name, pop, "ble");
#else
  Serial.println("BLE not enabled. Check configuration.");
#endif
}

void loop() {

}

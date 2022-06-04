const char *secret_ssid = "";
const char *secret_password = "";
const char *server = "example.com";
const char *auth_token = ""; // get it from stadtpuls.com
char sensor_id[] = "100000"; // get it from stadtpuls.com
String path = "/api/v3/sensors/" + String(sensor_id) + "/records";
char sensor_name[] = "Cloak";
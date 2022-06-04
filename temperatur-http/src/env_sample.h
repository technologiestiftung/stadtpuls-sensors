const char *secret_ssid = "";
const char *secret_password = "";
const char *server = "example.com";
const char *auth_token = ""; // get it from stadtpuls.com

// we put these into a different file for mass flashing
// you can keep them here
char sensor_id[] = "100000"; // get it from stadtpuls.com
char sensor_name[] = "Cloak";
String path = "/api/v3/sensors/" + String(sensor_id) + "/records";
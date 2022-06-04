char sensor_id[] = "125"; // get it from stadtpuls.com
char sensor_name[] = "Medusa";
String path = "/api/v3/sensors/" + String(sensor_id) + "/records";

const char *secret_ssid = "";
const char *secret_password = "";
const char *server = "example.com";
const char *auth_token = ""; // get it from stadtpuls.com

// if you want can can include a root ca for your server in here
// you also will have to remove the line
// client.setInsecure(); // skip verification
// and add
// client.setCACert(root_ca);// if you want to be save
// at the same location

// const char *root_ca =
//     "-----BEGIN CERTIFICATE-----\n" +
//     "MIIDBTCCAe2gAwIBAgIUCWMZsNeU2g5jKXIZdVW2/gwDRBEwDQYJKoZIhvcNAQEL\n" +
//     "BQAwEjEQMA4GA1UEAwwHemVpdC5jbzAeFw0xOTEwMDkxODU0MzlaFw0yMTEwMDgx\n" +
//     "ODU0MzlaMBIxEDAOBgNVBAMMB3plaXQuY28wggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" +
//     "DwAwggEKAoIBAQDWZyJvS/5WGoBw0YPt3VaP19BMAhRw99Biadyvk7YFUWyH1ivn\n" +
//     "vSItEYNKg9Asnyn/rg8L5E8ozoWmimc71mQIsBmsCZX7L+PhJB3Q4j8Bowjk0SN+\n" +
//     "F6SZhT5JUVfm+viLAdKAMsWDVM0PsL7SIQMhGl4y0Bi5Rwk7zp7Le9dmiE50kLz9\n" +
//     "n+0yNaQ9e9pbKnqk3PFP/HFaIOY0ytQtQOaZ8HJa1vI8Jtc8zg5ZbvQN7s4U+5x1\n" +
//     "dbwbgn+uICfoBQygHJvce7QSl/wSXdQRNO2ASibPJiwjIwzl96eI0rALktwKUry7\n" +
//     "yT6q3GW+Ct6J1XBojOLxMLgRV+yWmfWqDgdfAgMBAAGjUzBRMB0GA1UdDgQWBBRQ\n" +
//     "HJtdO8Li96r+DaeOAF+PJO0ojDAfBgNVHSMEGDAWgBRQHJtdO8Li96r+DaeOAF+P\n" +
//     "JO0ojDAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQDTfrz+3zPV\n" +
//     "XvjlFgr0PzF2SllGyUUirAmWh6rEdzv0RAPI+YH5Ff7RQTPSG5ZI97a4a5+2gxxV\n" +
//     "82KOzKYFkklficoks5mLx/MmTWMzrDcgpLGQpYzOThkP6z97cMiK6lL8pLMHX0+m\n" +
//     "wENXkOGYU6wDv8B/jt0xejNqmZnWyVq9N77cBZwrZnHL8J/WUvuYzNG2p8Yj27fj\n" +
//     "r2nCwaMeoA30A/3Sc4GT1oJSBwC05oZBsRQle6KsKXcfXYqQiSp3TGs+aJjwwYRk\n" +
//     "f0Ia6Op2MMDXtfMoNV7Z/3bhSrGp3ynEzreGVdRr/kWkyWYoFUTWbMEF7DG8/2Cw\n" +
//     "PXDViKe7nQpO\n" +
//     "-----END CERTIFICATE-----\n";

// we put these into a different file for mass flashing
// see util/sensorsenv_<NAME>.h
// you can keep them here
// char sensor_id[] = "100000"; // get it from stadtpuls.com
// char sensor_name[] = "Cloak";
// String path = "/api/v3/sensors/" + String(sensor_id) + "/records";

//--------------------------------------------------------------------------------------------
//                HTTP Credentials --> Create a token and sensor in stadtpuls
//--------------------------------------------------------------------------------------------
int SENSOR_PIN = 1;

const char *ssid0 = "SSID_NAME_1";
const char *pass0 = "PASSWORD_1";

const char *ssid0 = "SSID_NAME_2";
const char *pass0 = "PASSWORD_2";

const char *server = "api.stadtpuls.com";
const char *path = "/api/v3/sensors/ID_HERE/records";
const char *authToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6wefefwef.eyJzdWIiOiIyYmMzZTI2Zi0wZGZlLTQxMDAtOGRmYy1lMmEwYTBkODljNTAiLCJzY29wZSI6InN1ZG8iLCJkZXNjcmlwdGlvbiI6IlNlbnNvcnMgLSBXcmFuZ2Vsa2lleiBTb3VuZHMgLSBTdG9yeSIsImp0aSI6IjE3MTE5NGI0LWI2YTgtNGEyZi05ZTcwLTE0ZDdkZmVjNTRmMiIsImlzcyI6InN0YWR0cHVscy5jb20iLCJpwefwfwefewfwef23r._0PwzwMEQPGWDfK7-w43t2f3r233r";
const char *root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "9pbDH55GEgF+WjBTXTQcFeIS8vGLlfCU+K3jkw==\n"
    "-----END CERTIFICATE-----\n";

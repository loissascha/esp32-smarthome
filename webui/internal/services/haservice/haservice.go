package haservice

const token string = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI5ZTgzNDEzZjYxN2Q0MjE3YTJiOGM0NzIyZDI2ZmQ2ZSIsImlhdCI6MTc2OTY5OTc5NywiZXhwIjoyMDg1MDU5Nzk3fQ.8XaS8S2UCFTzdyc4yK5t3L_9Cdkeiv1gCwfayPuzr-I"

var registeredLights = []string{"light.tischlampe", "light.stehlampe"}
var registeredThermostates = []string{"climate.wohnzimmer_thermostat"}

type HAService struct {
}

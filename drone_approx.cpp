/* 
    Hardware-dependent interfacing is omitted and provided as an API as function implementation
    or code snippet injection. (UART, ISR registration and other sensor interfacing)
*/


/* Mode definitions */
#define MODE_INIT 0
#define MODE_MANUAL 1
#define MODE_GPS 2
#define MODE_LAND 3

static bool shutdown = false;
static int mode = MODE_INIT;

#define MAX_PITCH 45
#define MAX_YAW 45
#define MAX_ROLL 45
#define MAX_THRUST 100

#define IMU_TIMES 2 // IMU read times per loop
#define RC_CHANNELS 8
#define RC_THRUST 0
#define RC_PITCH 1
#define RC_YAW 2
#define RC_ROLL 3
#define RC_MODE 4

static bool imu_read_ready = false;
static bool gps_read_ready = false;

static void isr_uart_imu()
{
    imu_read_ready = true;
}

static void isr_uart_gps()
{
    gps_read_ready = true;
}

/* buffers */
/* Supposed pre-implemented data types */
static Vector3 acc[IMU_TIMES];
static Vector3 rot[IMU_TIMES];
static GpsPos gps;
static int rc_in[RC_CHANNELS];

/* PWM pulses duration */
#define MOTOR_OUT_MIN 0
#define MOTOR_OUT_MAX 1000
static int motor_out[4];

/* makes a dump of all the current state in the black box */
static void log_tick()
{
}

static void log_raw(const char *reason)
{
    // writes raw string value to log
}

static void error(const char *reason)
{
    log_raw(reason);
    for (int i = 0; i < 3; i++)
    {
        motor_out[i] = MOTOR_OUT_MIN;
    }

    motor_out_write(); 
    /* 
     * If this does not work, will keep recursively retrying
     * Configure ESC to turn motors off after specific time of no instruction.
     * Default is to hold current PWM, which can make the drone fly further away
     */

    // maybe notify about error maybe with telemetry
    while (true)
    { /* it's bad */
    }
}

static void sensors_read()
{
    for (int i = 0; i < IMU_TIMES; i++)
    {
        unsigned long start_time = millis(); // millis() should return current up-time to millis
        while (!imu_read_ready && (millis() - start_time) < 250)
        {
            /* busy-wait for interrupt */
        }

        if (!imu_read_ready)
        {
            error("IMU read timeout"); // disconnected wire
        }

        // imu actual value read and save to acc[i], rot[i]
    }
    // same logic for gps, but without timeout check

    // rc read from ibus + map to 0-100 range (from 1000-3000)
}

/* writes motor_out to the 4 escs via BLHeli */
static void motor_out_write()
{
}

#define P 1
#define I 2
#define D 3

static void control()
{
    float current_points[4] = {1,2,3,0}; // calculate from gyro and accel
    static float errors[4] = {0,0,0,0};
    static float target_points[4] = {0, 0, 0, 0}; // thrust, pitch, yaw, roll

    auto prev_mode = mode;
    mode = rc_in[RC_MODE] % (2000 / 4);

    switch (mode)
    {
    case MODE_INIT:
        for (int i = 0; i < 3; i++)
        {
            motor_out[i] = MOTOR_OUT_MIN; // or 0, might change
        }
        break;
    case MODE_MANUAL:
        current_points[0] = rc_in[RC_THRUST] * MAX_THRUST; // on other modes this is calculated from setpoints and auto-land procedure
        current_points[1] = rc_in[RC_PITCH] * MAX_PITCH;
        // can trim these 2 up to 45 deg for more assistance or auto idle hover
        current_points[2] = rc_in[RC_YAW] * MAX_YAW;
        current_points[3] = rc_in[RC_ROLL] * MAX_ROLL;
        break;
    case MODE_LAND: /* omitted for brevity + simplicity */
        if (prev_mode != MODE_LAND) { // init mode land
            
        }
        break;
    case MODE_GPS:
        break;
    default:
        break; // maybe set mode to land and re-try (or hold current pos)
    }

    // actual pid_control
    for (int i = 0; i < 4; i++) {
        auto current_error = target_points[i] - current_points[i];
        
        // maybe calculate this in a more accurate way
        auto error_intergal = (errors[i] + current_error) / 2;
        auto error_derivative = (current_error - errors[i]) / 2;

        target_points[i] = P * current_error + I * error_intergal + D * error_derivative;

        errors[i] = current_error;
    }

    auto thrust = target_points[0];
    auto pitch = target_points[1];
    auto yaw = target_points[2];
    auto roll = target_points[3];

    // map to motor_out
    // assuming quadrotor with rotor indexing like on the report
    motor_out[0] = thrust + yaw + pitch + roll;
    motor_out[1] = thrust - yaw + pitch - roll;
    motor_out[2] = thrust - yaw - pitch + roll;
    motor_out[3] = thrust + yaw - pitch - roll;
}

static void init()
{
    /* hw-dependent interrupt registration */

    float angle = 0;
    do
    {
        for (int i = 0; i < IMU_TIMES; i++)
        {
            sensors_read(); // fill the buffers fully
        }

        for (int i = 0; i < IMU_TIMES - 1; i++)
        {
            angle = acc[i] * acc[i + 1]; // overloaded op: dot product
        }
        angle = angle / (IMU_TIMES - 1);
    } while (angle < 2); // suppose mean 2 degree angle is stable (?)
}

int main()
{
    bool usb_plugged = false;
    if (usb_plugged)
    {
        // maintainance_mode(); - turn to usb mode and write EEPROM preconfigured locations
        return 0;
    }

    /* 
     * Every stage of this can 'throw' and end in error(), spinning forever into oblivion
     * Maybe we need to do a best-effort try to return to home / gracefully emergency land
     * instead of just shutting down.
     */
    init();

    while (!shutdown)
    {
        sensors_read();
        control();
        motor_out_write();
        log_tick();
    }

    // other shutdown logic -- depends on use case or specific hw
    return 0;
}
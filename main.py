from time import sleep
import pigpio

def generate_ramp(ramp):
    """Generate ramp wave forms.
    ramp:  List of [Frequency, Steps]
    """
    pi.wave_clear()     # clear existing waves
    length = len(ramp)  # number of ramp levels
    wid = [-1] * length

    # Generate a wave per ramp level
    for i in range(length):
        frequency = ramp[i][0]
        micros = int(500000 / frequency)
        wf = []
        wf.append(pigpio.pulse(1 << STEP, 0, micros))  # pulse on
        wf.append(pigpio.pulse(0, 1 << STEP, micros))  # pulse off
        pi.wave_add_generic(wf)
        wid[i] = pi.wave_create()

    # Generate a chain of waves
    chain = []
    for i in range(length):
        steps = ramp[i][1]
        x = steps & 255
        y = steps >> 8
        chain += [255, 0, wid[i], 255, 1, x, y]

    pi.wave_chain(chain)  # Transmit chain.


print("Running...")

DIR = 21     # Direction GPIO Pin
STEP = 20    # Step GPIO Pin

# Connect to pigpiod daemon
pi = pigpio.pi()

# Set up pins as an output
pi.set_mode(DIR, pigpio.OUTPUT)
pi.set_mode(STEP, pigpio.OUTPUT)

MODE = (14, 15, 18)   # Microstep Resolution GPIO Pins
RESOLUTION = {'Full': (0, 0, 0),
              'Half': (1, 0, 0),
              '1/4': (0, 1, 0),
              '1/8': (1, 1, 0),
              '1/16': (0, 0, 1),
              '1/32': (1, 0, 1)}
for i in range(3):
    pi.write(MODE[i], RESOLUTION['1/8'][i])

# Set duty cycle and frequency
#pi.set_PWM_dutycycle(STEP, 128)  # PWM 1/2 On 1/2 Off
#pi.set_PWM_frequency(STEP, 1600)  # 500 pulses per second

try:
    #pi.set_PWM_dutycycle(STEP, 128)
    #pi.set_PWM_frequency(STEP, 500)  

    #sleep(1)

    generate_ramp([
           [1600, 1000],
           [2000, 1000],
           [2500, 1000],
           [4000, 1000],
           [5000, 1000],
           [8000, 10000],
           [10000, 10000],
           [12000, 20000],
           [14000, 20000],
           [15000, 20000],
           [16000, 10000],
           [17000, 20000],
           [20000, 10000],
           [14000, 10000],
           [13000, 20000],
           [10000, 20000],])

    
    #pi.set_PWM_dutycycle(STEP, 128)
    #pi.set_PWM_frequency(STEP, 1000)

    while True:
        sleep(.1)

except KeyboardInterrupt:
    print ("\nCtrl-C pressed.  Stopping PIGPIO and exiting...")
finally:
    pi.set_PWM_dutycycle(STEP, 0)  # PWM off
    pi.stop()
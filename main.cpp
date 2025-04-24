import RPi.GPIO as GPIO
import time

troyka_pin = 13  # Твой тройка-пин

GPIO.setmode(GPIO.BCM)
GPIO.setup(troyka_pin, GPIO.OUT)

try:
    GPIO.output(troyka_pin, GPIO.HIGH)
    print("Питание подано на тройка-пин (GPIO 13)")
    input("Нажми Enter, чтобы отключить питание...")

finally:
    GPIO.output(troyka_pin, GPIO.LOW)
    GPIO.cleanup()
    print("Питание отключено, GPIO очищены")

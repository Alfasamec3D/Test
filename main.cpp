import RPi.GPIO as GPIO
import matplotlib.pyplot as plt
import time

# --- Константы ---
DAC_PINS = [26, 19, 13, 6, 5, 11, 9, 10]         # Пины ЦАП
LED_PINS = [24, 25, 8, 7, 12, 16, 20, 21]        # Пины светодиодов
COMP_PIN = 4                                     # Пин компаратора
TROYKA_PIN = 17                                  # Пин тройка-модуля

BITS = len(DAC_PINS)
LEVELS = 2 ** BITS
MAX_VOLTAGE = 3.3

# --- Настройка GPIO ---
GPIO.setmode(GPIO.BCM)
GPIO.setup(DAC_PINS, GPIO.OUT)
GPIO.setup(LED_PINS, GPIO.OUT)
GPIO.setup(TROYKA_PIN, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(COMP_PIN, GPIO.IN)


# --- Функции ---

def dec_to_bin(value):
    """Преобразует десятичное значение в список из 8 бит"""
    return [int(bit) for bit in bin(value)[2:].zfill(BITS)]

def adc():
    """Измеряет аналоговое напряжение с помощью последовательного приближения"""
    value = 0
    for i in range(BITS - 1, -1, -1):
        value += 2 ** i
        GPIO.output(DAC_PINS, dec_to_bin(value))
        time.sleep(0.001)
        if GPIO.input(COMP_PIN) == 0:
            value -= 2 ** i
    return value

def output_to_leds(value):
    """Отображает значение на светодиодах"""
    GPIO.output(LED_PINS, dec_to_bin(value))

# --- Главный блок ---
try:
    data = []
    timestamps = []

    print(">>> Начало эксперимента (зарядка конденсатора)...")
    GPIO.output(TROYKA_PIN, 1)
    start_time = time.time()

    value = 0
    while value < int(LEVELS * 0.97):
        value = adc()
        voltage = value / LEVELS * MAX_VOLTAGE
        output_to_leds(value)
        data.append(value)
        timestamps.append(time.time() - start_time)

    print(">>> Начало разрядки конденсатора...")
    GPIO.output(TROYKA_PIN, 0)

    while value > int(LEVELS * 0.02):
        value = adc()
        voltage = value / LEVELS * MAX_VOLTAGE
        output_to_leds(value)
        data.append(value)
        timestamps.append(time.time() - start_time)

    end_time = time.time()

    # --- Вывод параметров эксперимента ---
    duration = end_time - start_time
    sampling_period = duration / len(data)
    sampling_frequency = 1 / sampling_period
    quantization_step = MAX_VOLTAGE / LEVELS

    print("\n>>> Эксперимент завершён")
    print(f"Общая длительность: {duration:.3f} сек")
    print(f"Период одного измерения: {sampling_period:.5f} сек")
    print(f"Средняя частота дискретизации: {sampling_frequency:.2f} Гц")
    print(f"Шаг квантования: {quantization_step:.5f} В")

    # --- Сохранение результатов ---
    with open("data.txt", "w") as file:
        file.write("\n".join([str(v) for v in data]))

    with open("settings.txt", "w") as file:
        file.write(f"Частота дискретизации: {sampling_frequency:.2f} Гц\n")
        file.write(f"Шаг квантования: {quantization_step:.5f} В\n")

    # --- Построение графика ---
    plt.plot(timestamps, data)
    plt.title("Измерения напряжения на RC-цепи")
    plt.xlabel("Время, с")
    plt.ylabel("Цифровое значение АЦП")
    plt.grid(True)
    plt.show()

finally:
    # --- Очистка GPIO ---
    GPIO.output(DAC_PINS, GPIO.LOW)
    GPIO.output(LED_PINS, GPIO.LOW)
    GPIO.output(TROYKA_PIN, GPIO.LOW)
    GPIO.cleanup()
    print("GPIO очищены")

import random

def generate_data():
    first_number = random.randint(0,65535)
    second_numbers = [random.randint(-32768, 32767) for _ in range(1000)]
    return f"{first_number} {' '.join(map(str, second_numbers))}"

def generate_datasheet(filename):
    with open(filename, "w") as f:
        for _ in range(1000):
            f.write(generate_data() + '\n')

i = 1
while True:
    filename = f"datasheet-{i}.txt"
    try:
        generate_datasheet(filename)
        break
    except FileExistsError:
        i += 1
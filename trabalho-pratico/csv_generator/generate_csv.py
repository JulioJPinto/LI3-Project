import argparse
import string
import random

cities = ['Lisboa', 'Porto', 'Coimbra', 'Braga', 'Faro', 'Setubal', 'Viseu', 'Aveiro', 'Braganca', 'Leiria']

first_names = ['Joao', 'Maria', 'Jose', 'Ana', 'Manuel', 'Rita', 'Carlos', 'Sofia', 'Pedro', 'Ines']
last_names = ['Silva', 'Santos', 'Ferreira', 'Lopes', 'Pereira', 'Almeida', 'Rocha', 'Costa', 'Gomes', 'Martins']


def rand():
    return random.random()


def randint(start=0, end=1):
    return start + int(rand() * (end - start))


def choice(values):
    return values[int(rand() * len(values))]


def choice_prob(options, probs):
    x = rand()
    cum = 0
    index = 0

    for index, p in enumerate(probs):
        cum += p
        if x < cum:
            break
    return options[index]


def generate_random_first_name():
    return choice(first_names)


def generate_random_last_name():
    return choice(last_names)


def generate_random_city():
    return choice(cities)


def generate_day_or_month(start=1, end=12):
    x = randint(start, end)
    if x < 10:
        return f'0{x}'
    return str(x)


def generate_random_date(start_year=1950, end_year=1999):
    return '/'.join([generate_day_or_month(1, 28), generate_day_or_month(1, 12), str(randint(start_year, end_year))])


alphanumeric_characters_list = list(string.ascii_uppercase + string.digits)


def generate_random_license_plate():
    return choice(alphanumeric_characters_list) + choice(alphanumeric_characters_list) + "-" + \
           choice(alphanumeric_characters_list) + choice(alphanumeric_characters_list) + "-" + \
           choice(alphanumeric_characters_list) + choice(alphanumeric_characters_list)


def generate_driver_column(driver_id):
    return [str(driver_id).zfill(12),
            generate_random_first_name() + " " + generate_random_last_name(),
            generate_random_date(),
            choice(['M', 'F']),
            choice(['basic', 'premium', 'green']),
            generate_random_license_plate(),
            generate_random_city(),
            generate_random_date(2010, 2021),
            choice_prob(['active', 'inactive'], probs=[0.99, 0.01])]


def generate_user_column(username):
    return [username,
            generate_random_first_name() + " " + generate_random_last_name(),
            choice(["M", "F"]),
            generate_random_date(1950, 2005),
            generate_random_date(2010, 2021),
            choice(['cash', 'debit_card', 'credit_card']),
            choice_prob(["active", "inactive"], probs=[0.99, 0.01])]


def generate_ride_column(ride_id, driver_id, username):
    return [str(ride_id).zfill(12),
            generate_random_date(2010, 2021),
            str(driver_id).zfill(12),
            username,
            generate_random_city(),
            str(randint(1, 15)),
            str(randint(1, 5)),
            str(randint(1, 5)),
            str(randint(1, 10) / 2),
            ""]


def generate_random_username():
    first_name = generate_random_first_name()[0:randint(1, 5)]
    last_name = generate_random_last_name()
    user_random_number = str(randint(1, 100000000))
    return first_name + last_name + user_random_number


def generate_users_csv(filename, num_users):
    generated_users = set()

    with open(filename, 'w') as f:
        f.write('username;name;gender;birth_date;account_creation;pay_method;account_status\n')
        for i in range(num_users):
            username = generate_random_username()
            while username in generated_users:
                username = generate_random_username()

            generated_users.add(username)

            f.write(";".join(generate_user_column(username)) + "\n")

    return list(generated_users)


def generate_drivers_csv(filename, num_drivers):
    with open(filename, 'w') as f:
        f.write('id;name;birth_day;gender;car_class;license_plate;city;account_creation;account_status\n')
        for i in range(num_drivers):
            f.write(";".join(generate_driver_column(i + 1)) + "\n")

    return range(1, num_drivers)


def generate_rides_csv(filename, num_rides, drivers, users):
    with open(filename, 'w') as f:
        f.write('id;date;driver;user;city;distance;score_user;score_driver;tip;comment\n')
        for i in range(num_rides):
            f.write(";".join(generate_ride_column(i + 1, choice(drivers), choice(users))) + "\n")

    return range(1, num_rides)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-users-size', help='Number of lines of users.csv')
    parser.add_argument('-drivers-size', help='Number of lines of drivers.csv')
    parser.add_argument('-rides-size', help='Number of lines of rides.csv')
    parser.add_argument('-output-dir', help='Output directory')
    args = parser.parse_args()

    users_size = int(args.users_size)
    drivers_size = int(args.drivers_size)
    rides_size = int(args.rides_size)
    output_dir = args.output_dir

    users = generate_users_csv(output_dir + '/users.csv', users_size)
    drivers = generate_drivers_csv(output_dir + '/drivers.csv', drivers_size)
    generate_rides_csv(output_dir + '/rides.csv', rides_size, drivers, users)


if __name__ == '__main__':
    main()

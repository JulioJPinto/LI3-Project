use std::collections::{HashSet};
use std::fs;
use std::fs::{File, OpenOptions};
use std::io::{BufWriter, Write};
use clap::{Parser};
use rand::Rng;

static CITIES: [&str; 10] = ["Lisboa", "Porto", "Coimbra", "Braga", "Faro", "Setubal",
    "Viseu", "Aveiro", "Braganca", "Leiria"];

static FIRST_NAMES: [&str; 20] = ["Joao", "Maria", "Jose", "Ana", "Manuel", "Rita", "Carlos", "Sofia", "Pedro", "Ines",
    "Antonio", "Luisa", "Paulo", "Clara", "Miguel", "Andreia", "Rui", "Patricia", "Francisco", "Carla",
];
static LAST_NAMES: [&str; 20] = ["Silva", "Santos", "Ferreira", "Lopes", "Pereira", "Almeida", "Rocha", "Costa", "Gomes", "Martins",
    "Oliveira", "Correia", "Goncalves", "Ribeiro", "Alves", "Neves", "Soares", "Marques", "Teixeira", "Mendes"];

fn generate_random_date(start_year: u32, end_year: u32) -> String {
    let year = (rand::thread_rng().gen::<f32>() * start_year as f32) as u32 + (end_year - start_year);
    let month = (rand::thread_rng().gen::<f32>() * 11.0) as u32 + 1;
    let day = (rand::thread_rng().gen::<f32>() * 29.0) as u32 + 1;
    format!("{}/{}/{}", day, month, year)
}

fn generate_random_city() -> &'static str {
    CITIES[rand::thread_rng().gen_range::<usize, _>(0..CITIES.len())]
}

fn generate_random_name() -> String {
    let first_name = FIRST_NAMES[rand::thread_rng().gen_range::<usize, _>(0..FIRST_NAMES.len())];
    let last_name = LAST_NAMES[rand::thread_rng().gen_range::<usize, _>(0..LAST_NAMES.len())];
    format!("{} {}", first_name, last_name)
}

fn generate_random_username() -> String {
    let mut rng = rand::thread_rng();
    let random_number: u64 = rng.gen();
    let first_name_size = 3;
    let first_name = &FIRST_NAMES[rng.gen_range::<usize, _>(0..FIRST_NAMES.len())][..first_name_size];
    let last_name = LAST_NAMES[rng.gen_range::<usize, _>(0..LAST_NAMES.len())];
    format!("{first_name}{last_name}{random_number}")
}

static ALPHA_NUMERIC: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

fn generate_random_license_plate() -> String {
    let mut rng = rand::thread_rng();
    let mut license_plate = String::new();

    for _ in 0..2 {
        let idx = (rng.gen::<f32>() * (ALPHA_NUMERIC.len() - 1) as f32) as usize;
        license_plate.push(ALPHA_NUMERIC[idx] as char);
    }
    license_plate.push('-');
    for _ in 0..2 {
        let idx = (rng.gen::<f32>() * (ALPHA_NUMERIC.len() - 1) as f32) as usize;
        license_plate.push(ALPHA_NUMERIC[idx] as char);
    }
    license_plate.push('-');
    for _ in 0..2 {
        let idx = (rng.gen::<f32>() * (ALPHA_NUMERIC.len() - 1) as f32) as usize;
        license_plate.push(ALPHA_NUMERIC[idx] as char);
    }

    license_plate
}

fn generate_random_gender() -> &'static str {
    if rand::thread_rng().gen() { "M" } else { "F" }
}

fn generate_random_car_class() -> &'static str {
    let classes = ["basic", "premium", "green"];
    classes[rand::thread_rng().gen_range::<usize, _>(0..classes.len())]
}

fn generate_random_account_activeness() -> &'static str {
    if rand::thread_rng().gen::<f32>() < 0.99 { "active" } else { "inactive" }
}

fn generate_random_payment_method() -> &'static str {
    let methods = ["credit", "debit", "cash"];
    methods[rand::thread_rng().gen_range::<usize, _>(0..methods.len())]
}

fn generate_driver_line(id: usize) -> String {
    // id;name;birth_day;gender;car_class;license_plate;city;account_creation;account_status
    let name = generate_random_name();
    let username = generate_random_username();
    let birth_date = generate_random_date(1950, 2000);
    let gender = generate_random_gender();
    let car_class = generate_random_car_class();
    let license_plate = generate_random_license_plate();
    let city = generate_random_city();
    let account_creation = generate_random_date(2010, 2020);
    let account_status = generate_random_account_activeness();

    format!("{:012};{};{};{};{};{};{};{};{};{}", id, name, username, birth_date, gender, car_class,
            license_plate, city, account_creation, account_status)
}

fn generate_user_line(username: &str) -> String {
    //username;name;gender;birth_date;account_creation;pay_method;account_status
    let name = generate_random_name();
    let gender = generate_random_gender();
    let birth_date = generate_random_date(1950, 2000);
    let account_creation = generate_random_date(2010, 2020);
    let pay_method = generate_random_payment_method();
    let account_status = generate_random_account_activeness();

    format!("{username};{name};{gender};{birth_date};{account_creation};{pay_method};{account_status}")
}

fn generate_ride_line(ride_id: usize, driver_id: usize, username: &str) -> String {
    //id;date;driver;user;city;distance;score_user;score_driver;tip;comment
    let date = generate_random_date(2010, 2020);
    let city = generate_random_city();
    let distance = rand::thread_rng().gen_range::<u32, _>(1..15);
    let score_user = rand::thread_rng().gen_range::<u32, _>(0..5);
    let score_driver = rand::thread_rng().gen_range::<u32, _>(0..5);
    let tip = rand::thread_rng().gen_range::<u32, _>(0..10) as f32 / 2.0;

    format!("{:012};{date};{:012};{username};{city};{distance};{score_user};{score_driver};{tip};",
            ride_id, driver_id)
}

#[derive(Parser)]
struct Cli {
    users_size: usize,
    drivers_size: usize,
    rides_size: usize,
    output_dir: String,
}

fn open_file(folder: &str, path: &str) -> BufWriter<File> {
    BufWriter::new(
        OpenOptions::new()
            .write(true)
            .create(true)
            .read(false)
            .truncate(false)
            .open(format!("{folder}/{path}")).unwrap())
}

fn main() {
    let args = Cli::parse();

    fs::create_dir_all(&args.output_dir).unwrap();

    let mut users_file = open_file(&args.output_dir, "users.csv");
    let mut drivers_file = open_file(&args.output_dir, "drivers.csv");
    let mut rides_file = open_file(&args.output_dir, "rides.csv");

    drivers_file.write_all(b"id;name;birth_day;gender;car_class;license_plate;city;account_creation;account_status\n").unwrap();

    for i in 0..args.drivers_size {
        drivers_file.write(generate_driver_line(i).as_bytes()).unwrap();
        drivers_file.write("\n".as_bytes()).unwrap();
    }

    let mut users_array: Vec<String> = Vec::new();
    let mut users_set: HashSet<String> = HashSet::new();

    users_file.write_all(b"username;name;gender;birth_date;account_creation;pay_method;account_status\n").unwrap();

    for _i in 0..args.users_size {
        let mut username: String = generate_random_username();

        while users_set.contains(&username) {
            username = generate_random_username();
            println!("Username {} already exists, generating a new one", username);
        }

        users_file.write_all(generate_user_line(&username).as_bytes()).unwrap();
        users_file.write_all("\n".as_bytes()).unwrap();

        users_set.insert(username.clone());
        users_array.push(username);
    }

    rides_file.write_all(b"id;date;driver;user;city;distance;score_user;score_driver;tip;comment\n").unwrap();

    for i in 0..args.rides_size {
        let driver_id = rand::thread_rng().gen_range::<usize, _>(0..args.drivers_size);
        let username = &users_array[rand::thread_rng().gen_range::<usize, _>(0..args.users_size)];

        rides_file.write_all(generate_ride_line(i, driver_id, username).as_bytes()).unwrap();
        rides_file.write_all("\n".as_bytes()).unwrap();
    }

    users_file.flush().unwrap();
    drivers_file.flush().unwrap();
    rides_file.flush().unwrap();
}
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#define ZERO 0
#define ONE 1
#define TWO 2

#define SPACE ' '
#define BACKSPACE '\b'
#define NULL_STR ""

using namespace std;

const string FINISH = "finish";
const string CARWASH_INFO = "show_carwash_info";
const string STAGE_INFO = "show_stage_info";
const string TIME = "advance_time";
const string ADD_CAR = "add_car";
const string ADD_STAGE = "add_stage";

const string OK = "OK";
const string INVALID_COMMAND = "INVALID_COMMAND!";

const string PASSED_TIME_OUTPUT = "Passed time: ";
const string CARS_WAITING_OUTPUT = "Cars waiting: ";
const string STAGES_INFO_OUTPUT = "Stages info: ";
const string CARS_FINISHED_OUTPUT = "Cars finished: ";
const string CAR_ID_OUTPUT = "Car ID: ";
const string LUX_OUTPUT = "Luxury coefficient: ";
const string TIME_LEFT_OUTPUT = "Time left: ";
const string WORKER_ID_OUTPUT = "Worker ID: ";
const string FREE_OUTPUT = "Free";
const string STAGE_ID_OUTPUT = "Stage ID: ";
const string CARS_QUEUE_OUTPUT = "Cars in waiting queue: ";

class Car {
private:
    int id;
    int lux_coeff;
    int time_left;
    int worker_id;
public:
    Car(int _id, int _lux_coeff) {
        id = _id;
        lux_coeff = _lux_coeff;
    }

    Car() {};

    string get_car_info() {
        stringstream ss;
        ss << CAR_ID_OUTPUT << id << endl << LUX_OUTPUT << lux_coeff << endl << TIME_LEFT_OUTPUT << time_left;
        return ss.str();
    }

    void set_time_left(int _time_left) {
        time_left = _time_left;
    }

    void set_woker_id(int _worker_id) {
        worker_id = _worker_id;
    }

    int get_time_left() {return time_left;}

    int get_worker_id() {return worker_id;}
};

class Worker {
private:
    int id;
    int is_free;
    Car car;
    int coeff;
public:
    Worker(int _id, int _coeff) {
        id = _id;
        coeff = _coeff;
    }

    void set_car(Car _car) {
        car = _car;
    }

    void set_free(int num) {
        is_free = num;
    }

    int get_is_free() {return is_free;}

    int get_id() {return id;}

    int get_coeff() {return coeff;}

    Car get_car() {return car;}

    string get_worker_info() {
        stringstream ss;
        ss << WORKER_ID_OUTPUT << id << endl;
        if(is_free)
            ss << FREE_OUTPUT;
        else 
            ss << car.get_car_info();
        return ss.str();
    }
};

class Stage {
private:
    int id;
    vector<Worker> workers;
    vector<Car> waiting_queue;
public:
    Stage(int _id, vector<Worker> _workers) {
        id = _id;
        workers = _workers;
    }

    void set_waiting_queue(vector<Car> _waiting) {
        waiting_queue = _waiting;
    }

    int get_id() {return id;}

    vector<Car> get_waiting_queue() {return waiting_queue;}

    vector<Worker> get_workers() {return workers;}

    string get_stage_info() {
        stringstream ss;
        ss << STAGE_ID_OUTPUT << id << endl;
        for(auto x : workers) 
            ss << x.get_worker_info() << endl;
        ss << BACKSPACE;
        ss << CARS_QUEUE_OUTPUT;
        if(waiting_queue.size()) {
            for(auto x : waiting_queue)
                ss << x.get_car_info() << endl;
            ss << BACKSPACE;
        }
        return ss.str();
    }
};

string seperate_string(string line, char delimiter, int num) {
    stringstream ss(line);
    string word;
    for(int i = ZERO; i < num; i ++) 
        getline(ss, word, delimiter);
    return word;
}

int seperate_int(string line, char delimiter, int num) {
    stringstream ss(line);
    string word;
    for(int i = ZERO; i < num; i ++) 
        getline(ss, word, delimiter);
    return stoi(word);
}

void print_on_terminal(string output) {
    cout << output;
}

vector<int> find_coeffs_vec_index(string command_line, int worker_count, int num) {
    int time_coeff_worker;
    vector<int> coeffs_index;
    for(int i = ZERO; i < worker_count; i ++) {
        num ++;
        time_coeff_worker = seperate_int(command_line, SPACE, num);
        coeffs_index.push_back(time_coeff_worker);
    }
    return coeffs_index;
}

class Carwash {
private:
    int time_sim;
    int last_worker_id;
    int last_stage_id;
    vector<Stage> stages;
    vector<Car> finished_cars;
    vector<Car> waiting_cars_start;
    vector<Car> all_cars;
public:
    Carwash() {};

    void do_orders() {
        string command_line;
        string order;
        int stage_id, worker_count, time_coeff_car, num_advance_time;
        while(getline(cin, command_line)) {
            order = seperate_string(command_line, SPACE, ONE);
            if(order == CARWASH_INFO)
                make_output_carwash();
            else if(order == STAGE_INFO) {
                stage_id = seperate_int(command_line, SPACE, TWO);
                print_on_terminal(stages[stage_id].get_stage_info());
            }
            else if(order == ADD_STAGE) {
                //int temp = ONE;
                //worker_count = seperate_int(command_line, SPACE, temp);
                //vector<int> coeffs = find_coeffs_vec_index(command_line, worker_count, temp);
                //add_stage(worker_count, coeffs);
                make_output(ADD_STAGE);
            }
            else if(order == ADD_CAR) {
                //time_coeff_car = seperate_int(command_line, SPACE, TWO);
                //add_car(time_coeff_car);
                make_output(ADD_CAR);
            }
            else if(order == TIME) {
                num_advance_time = seperate_int(command_line, SPACE, TWO);
                //advance_time(num_advance_time);
                make_output(TIME);
            }
            else if(order == FINISH) {
                //finish_all_levels();
                make_output(FINISH);
            }
            else
                make_output(INVALID_COMMAND);
        }
    }

    void add_car(int time_coeff_car) {
        int car_index = all_cars.size();
        Car new_car(car_index, time_coeff_car);
        all_cars.push_back(new_car);
    }

    void add_stage(int worker_count, vector<int> coeffs) {
        vector<Worker> workers_vec;
        for(auto x : coeffs) {
            Worker new_worker(x, coeffs[x]);
            workers_vec.push_back(new_worker);
        }
        int new_stage_id = stages.size();
        stages.push_back(Stage(new_stage_id, workers_vec));
    }

    string make_output_all_stages() {
        stringstream ss;
        if(stages.size()) {
            ss << endl;
            for(auto x : stages)
                ss << x.get_stage_info() << endl; 
            ss << BACKSPACE;
        }
        else
            ss << NULL_STR;
        return(ss.str());
    }

    string make_output_cars(int vec_class_num) {
        stringstream ss;
        if(!vec_class_num) {
            if(waiting_cars_start.size()) {
            ss << endl;
            for(auto x : waiting_cars_start)
                ss << x.get_car_info() << endl;
            ss << BACKSPACE;
            }
            else
            ss << NULL_STR;
        }
        else {
            if(finished_cars.size()) {
            ss << endl;
            for(auto x : finished_cars)
                ss << x.get_car_info() << endl;
            ss << BACKSPACE;
            }
            else
            ss << NULL_STR;
        }
        return ss.str();
    }

    void make_output_carwash() {
        stringstream ss;
        ss << PASSED_TIME_OUTPUT << time_sim << endl;
        ss << CARS_WAITING_OUTPUT << make_output_cars(ZERO) << endl;
        ss << STAGES_INFO_OUTPUT << make_output_all_stages() << endl;
        ss << CARS_FINISHED_OUTPUT << make_output_cars(ONE);
        print_on_terminal(ss.str());
    }

    void make_output(string order) {
        if(order == ADD_CAR || order == ADD_STAGE || order == TIME || order == FINISH)
            print_on_terminal(OK);
        else
            print_on_terminal(INVALID_COMMAND);
    }
};
       
int main()
{
    Carwash carwash;
    carwash.do_orders();
    return ZERO;
}
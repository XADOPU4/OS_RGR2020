
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

struct Process_Data {
	int Num;
	int Pid;  //Идентиификатор процесса
	int A_time; //Время появления процесса
	int B_time; //Время выполнения процесса
	int Priority; //Приоритет процесса
	int F_time; //Время завершения процесса
	int R_time; //Оставшееся время в процессе выполнения
	int W_time; //Время ожидания
	int S_time; //Время старта процесса
	int Res_time;

};

struct Process_Data current;
typedef struct Process_Data P_d;

bool idsort(const P_d& x, const P_d& y)
{
	return x.Pid < y.Pid;
}

/*Сортировка по времени прибытия, если оно совпадает, то по приоритету, если и он совпадает, то по идентификатору процесса*/
bool arrivalsort(const P_d& x, const P_d& y)
{
	if (x.A_time < y.A_time)
		return true;
	else if (x.A_time > y.A_time)
		return false;
	if (x.Priority < y.Priority)
		return true;
	else if (x.Priority > y.Priority)
		return false;
	if (x.Pid < y.Pid)
		return true;

	return false;
}


bool Numsort(const P_d& x, const P_d& y)
{
	return x.Num < y.Num;
}

/*Соритровка по приоритету, если совпадает, то по идентификатору*/
struct comPare {
	bool operator()(const P_d& x, const P_d& y)
	{
		if (x.Priority > y.Priority)
			return true;
		else if (x.Priority < y.Priority)
			return false;
		if (x.Pid > y.Pid)
			return true;

		return false;

	}

};

/**To check the Input **/
void my_check(vector<P_d> mv)
{
	for (size_t i = 0; i < mv.size(); i++) {
		cout << " Pid :" << mv[i].Pid << " A_time : " << mv[i].A_time << " B_time : " << mv[i].B_time << " Priority : " << mv[i].Priority << endl;
	}

}

int main()
{
	int i;
	vector<P_d > input;
	vector<P_d> input_copy;
	P_d temp;
	int pq_process = 0; // for PQ process
	int rq_process = 0; // for RQ process
	int A_time;
	int B_time;
	int Pid;
	int Priority;
	int n;
	int clock;
	int total_exection_time = 0;
	
	cout << "Process count: ";
	cin >> n;
	for (i = 0; i < n; i++) {
		cout << "Process ID: ";
		cin >> Pid;
		cout << "Arrival time: ";
		cin >> A_time;
		cout << "Burst time: ";
		cin >> B_time;
		cout << "Priority(0-Highest): ";
		cin >> Priority;
		cout << endl;

		
		temp.Num = i + 1;
		temp.A_time = A_time;
		temp.B_time = B_time;
		temp.R_time = B_time;
		temp.Pid = Pid;
		temp.Priority = Priority;
		input.push_back(temp);
	}
	input_copy = input;
	std::sort(input.begin(), input.end(), arrivalsort);

	//my_check( input ); // To check the sort unomment it

	total_exection_time = total_exection_time + input[0].A_time;

	for (i = 0; i < n; i++) {
		if (total_exection_time >= input[i].A_time) {
			total_exection_time = total_exection_time + input[i].B_time;
		}
		else {
			int diff = (input[i].A_time - total_exection_time);
			total_exection_time = total_exection_time + diff + B_time;
		}
	}

	int* Gantt = new int[total_exection_time];//Gantt Chart

	for (i = 0; i < total_exection_time; i++) {
		Gantt[i] = -1;
	}


	priority_queue < P_d, vector<Process_Data>, comPare> pq; //Priority Queue PQ

	queue< P_d > rq; //Round Robin Queue RQ
	int cpu_state = 0; //0 - простаивает, 1 - работает
	int quantum = 4; //Time Quantum
	current.Pid = -2;
	current.Priority = 999999;

	for (clock = 0; clock < total_exection_time; clock++) {
		/*Добавление процессов на кванте, если совпадает время прибытия*/
		for (int j = 0; j < n; j++) {
			if (clock == input[j].A_time) {
				pq.push(input[j]);
			}
		}


		if (cpu_state == 0) //Если процессор свободен
		{
			if (!pq.empty()) {
				current = pq.top();
				cpu_state = 1;
				pq_process = 1;
				pq.pop();
				quantum = 4;
			}
			else if (!rq.empty()) {
				current = rq.front();
				cpu_state = 1;
				rq_process = 1;
				rq.pop();
				quantum = 4;
			}
		}
		else if (cpu_state == 1) //Если процессор что-то выполняет
		{
			if (pq_process == 1 && (!pq.empty())) {
				if (pq.top().Priority < current.Priority) //Если у прибывшего процесса приоритет выше
				{
					rq.push(current); //текущий добавляется в RQ
					current = pq.top();
					pq.pop();
					quantum = 4;
				}
			}
			else if (rq_process == 1 && (!pq.empty())) // Если текущий процесс в RQ, а прибыл процесс с приоритетом
			{
				rq.push(current);
				current = pq.top();
				pq.pop();
				rq_process = 0;
				pq_process = 1;
				quantum = 4;
			}


		}


		if (current.Pid != -2) // Выполнение процесса
		{
			current.R_time--;
			quantum--;
			Gantt[clock] = current.Pid;
			if (current.R_time == 0) //Если процесс выполнился
			{
				cpu_state = 0;
				quantum = 4;
				current.Pid = -2;
				current.Priority = 999999;
				rq_process = 0;
				pq_process = 0;
			}
			else if (quantum == 0) //Если кончились кванты для текущего процесса
			{
				rq.push(current);
				current.Pid = -2;
				current.Priority = 999999;
				rq_process = 0;
				pq_process = 0;
				cpu_state = 0;

			}

		}

	}


	std::sort(input.begin(), input.end(), idsort);

	for (int i = 0; i < n; i++) {
		for (int k = total_exection_time; k >= 0; k--) {
			if (Gantt[k] == i + 1) {
				input[i].F_time = k + 1;
				break;

			}
		}
	}
	for (int i = 0; i < n; i++) {
		for (int k = 0; k < total_exection_time; k++) {

			if (Gantt[k] == i + 1) {
				input[i].S_time = k;
				break;
			}
		}
	}

	std::sort(input.begin(), input.end(), Numsort);


	for (int i = 0; i < n; i++) {
		input[i].Res_time = input[i].S_time - input[i].A_time;
		input[i].W_time = (input[i].F_time - input[i].A_time) - input[i].B_time;

	}

	for (int i = 0; i < n; i++) {
		cout << "PID : "<<input[i].Pid << " R_time : " << input[i].Res_time << " F_Time : " << input[i].F_time << " W_Time : " << input[i].W_time << endl;

	}
	return 0;
}
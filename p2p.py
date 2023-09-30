import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

# Указывать общий объем собираемых данных
total_data = 100

# Создание таймера
comm.Barrier()
start_time = MPI.Wtime()

data = np.random.randint(0, 100, total_data // size) # Возвращает массив целых чисел в указанном диапазоне

# Корневой процесс собирает все данные
if rank == 0:
    result = []
    for i in range(1, size):
        # result = np.empty(total_data, np.int)
        received_date = np.empty(total_data // size, int)
        comm.Recv(received_date, source=i)
        result.append(received_date)
else:
    # print("I am ", rank, " of ", size, "my data: ", data)
    result = None
    comm.Send(data, dest=0)

# Остановить таймер
comm.Barrier()
elapsed_time = MPI.Wtime() - start_time

if rank == 0:
    print("\nresult : ", result)
    print("Total time: ", elapsed_time)

# to run: mpiexec -n 10 python p2p.py
# Total time:  0.0016275000234600157

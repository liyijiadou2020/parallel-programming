from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

# Указывать общий объем собираемых данных
total_data = 100
numDataPerRank = 10

# Создание таймера
comm.Barrier()
start_time = MPI.Wtime()

sendbuf = np.random.randint(0, 100, total_data // size) # Возвращает массив целых чисел в указанном диапазоне

recvbuf = None
if rank == 0:
    recvbuf = np.empty(numDataPerRank * size, int)

comm.Gather(sendbuf, recvbuf, root=0)

# Остановить таймер
comm.Barrier()
elapsed_time = MPI.Wtime() - start_time

if rank == 0:
    # print('Rank: ', rank, ', recvbuf received: ', recvbuf)
    print("\nresult: ", recvbuf)
    print("Total time: ", elapsed_time)

# to run: mpiexec -n 10 python gather.py
# Total time:   0.0007838000019546598

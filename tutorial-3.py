import threading
import time


class WorkerThread(threading.Thread):
    def __init__(self, name):
        super(WorkerThread, self).__init__(name=name)
        self.stop_event = threading.Event()

    def run(self):
        while not self.stop_event.is_set():
            print self.name, "is working"
            time.sleep(1)

    def stop(self):
        print self.name, "stop"
        self.stop_event.set()


class ThreadManager(object):
    def __init__(self):
        self.worker = WorkerThread("worker")

    def start_thread(self):
        self.worker.start()

    def stop_thread(self):
        self.worker.stop()
        self.worker.join()


def main():
    tm = ThreadManager()
    tm.start_thread()
    time.sleep(5)
    tm.stop_thread()

if __name__ == '__main__':
    main()

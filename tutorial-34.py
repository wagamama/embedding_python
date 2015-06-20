import threading
import time

def working(name):
    print name, "is working"
    time.sleep(1)

class WorkerThread(threading.Thread):
    def __init__(self, name):
        super(WorkerThread, self).__init__(name=name)
        self.stop_event = threading.Event()

    def run(self):
        while not self.stop_event.is_set():
            working(self.name)

    def stop(self):
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

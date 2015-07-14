import threading
import time


class WorkerThread(threading.Thread):
    def __init__(self, name, callback):
        super(WorkerThread, self).__init__(name=name)
        self.callback = callback
        self.stop_event = threading.Event()

    def run(self):
        while not self.stop_event.is_set():
            self.callback(self.name)
            time.sleep(1)

    def stop(self):
        self.stop_event.set()


class ThreadManager(object):
    def __init__(self, callback):
        self.worker1 = WorkerThread("worker1", callback)
        self.worker2 = WorkerThread("worker2", callback)

    def start_thread(self):
        self.worker1.start()
        self.worker2.start()

    def stop_thread(self):
        self.worker1.stop()
        self.worker2.stop()
        self.worker1.join()
        self.worker2.join()


def main():
    def callback(name):
        print name, "is working"

    tm = ThreadManager(callback)
    tm.start_thread()
    time.sleep(5)
    tm.stop_thread()

if __name__ == '__main__':
    main()

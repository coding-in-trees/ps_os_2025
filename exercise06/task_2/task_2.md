Instead of a mutex, could a semaphore be used in this situation?

Yes you could potentially use a semaphore but it would be way less robust and more unsafe than a mutex operation with its ownership principle.
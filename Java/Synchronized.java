public class Synchronized {
    static int i = -1;
    static int k = -1;

    public static void main(String[] args) {
        MeuRunnable runnable = new MeuRunnable();
        String name = Thread.currentThread().getName();
        System.out.println(name + "Linha principal");

        Thread t0 = new Thread(runnable);
        Thread t1 = new Thread(runnable);
        Thread t2 = new Thread(runnable);
        Thread t3 = new Thread(runnable);
        Thread t4 = new Thread(runnable);

        t0.start();
        t1.start();
        t2.start();
        t3.start();
        t4.start();
    }

    public static class MeuRunnable implements Runnable {
        static Object lock1 = new Object();
        static Object lock2 = new Object();

        @Override
        public void run() {
            String name = Thread.currentThread().getName();
            synchronized (lock1) { // lock1
                for (int j = 0; j < 1000; j++, i++)
                    ;
                System.out.println(name + ": " + i);
            }

            synchronized (lock2) { // lock1
            for (int j = 0; j < 1000; j++, k++)
                    ;
                System.out.println(name + ": " + k);
            }
        }
    }
}
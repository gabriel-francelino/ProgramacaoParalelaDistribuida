package org.example;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Server {
    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.createRegistry(1099);
            Naming.rebind("rmi://192.168.15.150:1099/Ola", new OlaImpl());
            System.out.println("Servidor levantou...");

        } catch (RemoteException | MalformedURLException e) {
            e.printStackTrace();
//            throw new RuntimeException(e);
        }
    }
}

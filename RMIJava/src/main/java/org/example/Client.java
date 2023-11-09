package org.example;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class Client {
    public static void main(String[] args) {
        try {
            Ola stub = (Ola) Naming.lookup("rmi://192.168.15.151:1099/Hello");
            System.out.println(stub.digaOla());
        } catch (RemoteException | NotBoundException | MalformedURLException e) {
            e.printStackTrace();
            throw new RuntimeException(e);
        }
    }
}

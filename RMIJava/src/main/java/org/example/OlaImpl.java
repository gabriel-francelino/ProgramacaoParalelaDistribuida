package org.example;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class OlaImpl extends UnicastRemoteObject implements Ola {
    protected OlaImpl() throws RemoteException {
        super();
    }

    private static final long serialVersionIUD = 1L;

    @Override
    public String digaOla() throws RemoteException {
//        throw new UnsupportedOperationException(message:"Unimplement method");
        return "Ola Mundo!";
    }
}

package de.flo.bwinf.task5;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class Writer {

    private FileWriter fileWriter;

    public Writer(String fileName) {
        this.create(fileName);
    }

    public void close() {
        try {
            this.fileWriter.close();
        } catch (IOException ignored) {
        }
    }

    public void write(String str) {
        try {
            this.fileWriter.write(str);
        } catch (IOException ignored) {
        }
    }

    private void create(String filename) {
        try {
            File file = new File(filename);

            this.fileWriter = new FileWriter(file);
        } catch (IOException ignored) {
        }
    }
}

package de.flo.hexMax.util;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * Class containing generally java related utility functions.
 */
public class JavaUtils {

    /**
     * Prevent making objects of this class since it is a utility class
     */
    private JavaUtils() {
    }

    /**
     * Function for sorting a collection by a property of its elements.
     * @param collection The collection that will be sorted
     * @param function Function for getting the elements' property
     * @param comparator Comparator for comparing the elements' properties
     * @param <T> The collection's type
     * @param <U> The property's type
     * @return A sorted list
     */
    public static <T, U> List<T> sortByProperty(Collection<T> collection, Function<T, U> function, Comparator<? super U> comparator) {
        return collection.stream().sorted(Comparator.comparing(function, comparator)).collect(Collectors.toList());
    }

    /**
     * Function for getting an array as List
     * @param array The array that will be returned as list
     * @param <T> The array's and list's type
     * @return A new list which array's content
     */
    public static <T> List<T> getAsList(T[] array) {
        return Arrays.stream(array).collect(Collectors.toList());
    }

    /**
     * Function for creating a Map based on a collection
     * @param collection The collection the map will be based on
     * @param keyFunction Function to get a key for map from collection
     * @param valueFunction Function to get a value for map from collection
     * @param <T> Collection's elements' type
     * @param <K> Map's keys' type
     * @param <V> Map's values' type
     * @return A new HashMap
     */
    public static <T, K, V> Map<K, V> createMap(Collection<T> collection, Function<T, K> keyFunction, Function<T, V> valueFunction) {
        return collection.stream().collect(Collectors.toMap(keyFunction, valueFunction));
    }

    /**
     * Static function for converting a List into an array, given the List and an empty array.
     * @param list The list that will be converted
     * @param emptyArr An empty array of the same type as the list
     * @param <T> The list's elements' type
     * @return A new Array
     */
    public static <T> T[] getAsArray(List<T> list, T[] emptyArr) {
        return list.toArray(emptyArr);
    }

    /**
     * Function to clone a List
     * @param collection Collection that will be cloned
     * @param func Function to convert from Collection to V
     * @param <T> Collections type
     * @param <V> Type extending Collection<T>
     * @return An Object of type V with elements of type T
     */
    public static <T, V extends Collection<T>> V clone(Collection<T> collection, Function<Collection<T>, V> func) {
        return func.apply(collection);
    }

    /**
     * Function for "reading" a given file
     * @param file File to read
     * @return Files content line by line as List of Strings
     * @throws FileNotFoundException If source is not found
     */
    public static List<String> readFile(File file) throws FileNotFoundException {
        List<String> lines = new ArrayList<>();

        Scanner sc = new Scanner(file);
        while (sc.hasNext())
            lines.add(sc.nextLine());

        return lines;
    }
}
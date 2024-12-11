using System;

public class DoublyLinkedList<T>
{
    private class Node
    {
        public T Data { get; set; }
        public Node Next { get; set; }
        public Node Previous { get; set; }

        public Node(T data)
        {
            Data = data;
            Next = null;
            Previous = null;
        }
    }

    private Node head;
    private Node tail;
    private Node cursor; // Represents the player's current position
    private int count;

    public int Count => count;

    public DoublyLinkedList()
    {
        head = null;
        tail = null;
        cursor = null;
        count = 0;
    }

    // Add a node to the end of the list
    public void Add(T data)
    {
        Node newNode = new Node(data);

        if (head == null)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail.Next = newNode;
            newNode.Previous = tail;
            tail = newNode;
        }

        count++;
    }

    // Reset the cursor to the start of the list
    public void ResetCursorToStart()
    {
        cursor = head;
    }

    // Move the cursor to the next node
    public bool MoveNext()
    {
        if (cursor == null || cursor.Next == null) return false;
        cursor = cursor.Next;
        return true;
    }

    // Move the cursor to the previous node
    public bool MovePrevious()
    {
        if (cursor == null || cursor.Previous == null) return false;
        cursor = cursor.Previous;
        return true;
    }

    // Get the current node's data
    public T GetCurrent()
    {
        if (cursor == null) throw new InvalidOperationException("Cursor is not pointing to a valid node.");
        return cursor.Data;
    }

    // Check if the cursor is valid
    public bool IsCursorValid()
    {
        return cursor != null;
    }
}

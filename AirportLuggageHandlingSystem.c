#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum size for names
#define NAME_SIZE 50

// Forward declarations of the structures and functions
typedef struct flightDetails FlightDetails;
typedef struct luggage Luggage;
typedef struct baggage Baggage;
typedef struct baggageCart BaggageCart;
typedef struct passengers Passengers;

Luggage *enterLuggage(Passengers *owner);
void beginBaggageLoadProcess(Luggage *newLuggage);
void beginBaggageUnloadProcess();
BaggageCart *createBaggageCart(Luggage *newLuggage);
Baggage *createBaggageQueue(Luggage *newLuggage);
void pushOnCart(Luggage *newLuggage);
void popFromCart();

// Structure to store flight details
struct flightDetails
{
    char airline[NAME_SIZE];
    int terminal;
};

// Structure to store luggage details
struct luggage
{
    char size[15];
    double weight;
    char ownerName[NAME_SIZE];
    Luggage *next;
    Passengers *owner;
};

// Structure to store baggage in a queue
struct baggage
{
    Luggage *luggage;
    Baggage *next;
};

// Structure to store baggage cart stack
struct baggageCart
{
    Luggage *luggage;
    BaggageCart *next;
};

// Structure to store passenger details
struct passengers
{
    char name[NAME_SIZE];
    int numOfBags;
    FlightDetails flightDetails;
    Passengers *next;
    Luggage *luggage;
};

// Global pointers for managing queues and stacks
Passengers *passengerFront = NULL;
Passengers *passengerRear = NULL;
Baggage *baggageFront = NULL;
Baggage *baggageRear = NULL;
BaggageCart *stackHead = NULL;

void clearInputBuffer()
{
    // Function to clear the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Function to enter passenger details
Passengers *enterPassenger()
{
    Passengers *newPassenger = (Passengers *)malloc(sizeof(Passengers));
    if (newPassenger == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    char passengerName[NAME_SIZE];
    int numOfBags;

    printf("Enter passenger name: ");
    fgets(passengerName, sizeof(passengerName), stdin);
    passengerName[strcspn(passengerName, "\n")] = 0;

    printf("Enter number of bags: ");
    scanf("%d", &numOfBags);
    clearInputBuffer();

    strcpy(newPassenger->name, passengerName);
    newPassenger->numOfBags = numOfBags;

    // Input for flight details
    printf("Enter airline name: ");
    fgets(newPassenger->flightDetails.airline, NAME_SIZE, stdin);
    newPassenger->flightDetails.airline[strcspn(newPassenger->flightDetails.airline, "\n")] = 0; // Remove newline character

    printf("Enter terminal number: ");
    scanf("%d", &newPassenger->flightDetails.terminal);
    clearInputBuffer();

    newPassenger->next = NULL;

    return newPassenger;
}

/**
 * Begins the check-in process for a new passenger.
 * Adds the passenger to the check-in queue and displays confirmation details.
 */
void beginCheckInProcess()
{
    Passengers *newPassenger = enterPassenger();
    if (newPassenger == NULL)
    {
        printf("\n[Check-In Error]");
        printf("\nUnable to start the check-in process due to an internal error.\n");
        return;
    }

    // Add the new passenger to the queue
    if (passengerFront == NULL)
    {
        passengerFront = passengerRear = newPassenger;
    }
    else
    {
        passengerRear->next = newPassenger;
        passengerRear = newPassenger;
    }

    // Display check-in confirmation details
    printf("\n[Check-In Confirmation]");
    printf("\nPassenger Name: %s", newPassenger->name);
    printf("\nAirline Name: %s", newPassenger->flightDetails.airline);
    printf("\nTerminal Number: %d", newPassenger->flightDetails.terminal);
    printf("\nStatus: Successfully entered for check-in.");
    printf("\nPlease proceed to the baggage drop-off area.\n");
}

/**
 * Completes the check-in process for a passenger.
 * Loads their luggage and updates the queue status.
 */
void completePassengerCheckIn()
{
    if (passengerFront == NULL && stackHead == NULL)
    {
        printf("\n[Check-In Status]");
        printf("\nThe passenger queue is currently empty. Starting baggage cart loading.\n");
        if (baggageFront == NULL)
        {
            printf("\nThe baggage queue is currently empty.\n");
        }
        else
        {
            while (baggageFront != NULL)
            {
                beginBaggageUnloadProcess();
            }
        }
        return;
    }

    if (passengerFront == NULL)
    {
        printf("\n[Check-In Status]");
        printf("\nNo passengers in the queue.\n");
        return;
    }

    Passengers *passenger = passengerFront;
    Luggage *newLuggage = enterLuggage(passenger);

    printf("\n[Check-In Status]");
    printf("\nPassenger Name: %s\n", passenger->name);
    if (passengerFront == passengerRear)
    {
        beginBaggageLoadProcess(newLuggage);
        passengerFront = passengerRear = NULL;
        printf("\nStatus: Checked in successfully. The queue is now empty.\n");
    }
    else
    {
        beginBaggageLoadProcess(newLuggage);
        passengerFront = passengerFront->next;
        printf("\nStatus: Checked in successfully. Next passenger, please come forward.\n");
    }
    passenger->luggage = newLuggage;
}

// Function to view the list of passengers
void viewPassengers()
{
    if (passengerFront == NULL)
    {
        printf("\n[Passenger List]");
        printf("\nNo passengers are currently checked in.\n");
        return;
    }
    printf("\n+----------------------------------------------------------------+");
    printf("\n| No. | Passenger Name       | Airline            | Terminal No. |");
    printf("\n+----------------------------------------------------------------+");

    Passengers *temp = passengerFront;
    int count = 1;
    while (temp != NULL)
    {
        printf("\n| %-3d | %-20s | %-18s | %-12d |", count, temp->name, temp->flightDetails.airline, temp->flightDetails.terminal);
        temp = temp->next;
        count++;
    }
    printf("\n+----------------------------------------------------------------+\n");
}

/**
 * Ends check-in process for a new passenger.
 */

/**
 * Begins the baggage queue for a new checked in passenger.
 */

// Function to enter luggage details
Luggage *enterLuggage(Passengers *owner)
{
    Luggage *newLuggage = (Luggage *)malloc(sizeof(Luggage));
    if (newLuggage == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    char size[15];
    double weight;

    printf("\n+------------------------------------------------------+");
    printf("\n| Passenger name: %-52s |", owner->name);
    printf("\n+------------------------------------------------------+");

    newLuggage->owner = owner;
    strcpy(newLuggage->ownerName, owner->name);

    printf("\nEnter luggage size: ");
    fgets(size, sizeof(size), stdin);
    strcpy(newLuggage->size, size);
    size[strcspn(size, "\n")] = 0;

    printf("Enter luggage weight (in kg): ");
    scanf("%lf", &newLuggage->weight);
    clearInputBuffer();

    newLuggage->next = NULL;

    return newLuggage;
}

// Function to create a baggage queue node
Baggage *createBaggageQueue(Luggage *newLuggage)
{
    Baggage *baggage = (Baggage *)malloc(sizeof(Baggage));
    if (baggage == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    baggage->luggage = newLuggage;
    baggage->next = NULL;
    return baggage;
}

// Function to begin the baggage load process
void beginBaggageLoadProcess(Luggage *newLuggage)
{
    Baggage *baggage = createBaggageQueue(newLuggage);
    printf("\n+------------------------------------------------------+");
    printf("\n|            Baggage Check-In Process Status           |");
    printf("\n+------------------------------------------------------+");

    if (baggage == NULL)
    {
        printf("\n| Error: Unable to start the check-in process due to    |");
        printf("\n|        an internal error. Please try again.           |");
        printf("\n+------------------------------------------------------+");
    }
    else
    {
        // Add the new luggage to the baggage queue
        if (baggageFront == NULL)
        {
            baggageFront = baggageRear = baggage;
            printf("\n| Success: Your luggage is the first in the queue!     |");
        }
        else
        {
            baggageRear->next = baggage;
            baggageRear = baggage;
            printf("\n| Success: Your luggage has been added to the queue.   |");
        }
        printf("\n+------------------------------------------------------+");
        printf("\n| %-20s | %-20s | %-10s |", "Luggage Owner", "Size", "Weight");
        printf("\n+------------------------------------------------------+");
        printf("\n| %-20s | %-20s | %-6.2f kg |", newLuggage->ownerName, newLuggage->size, newLuggage->weight);
    }
    printf("\n+------------------------------------------------------+");
    printf("\n");
}

// Function to begin the baggage unload process
void beginBaggageUnloadProcess() // Dequeue luggage from the baggage queue to the belt
{
    // Check if the baggage queue is empty
    if (baggageFront == NULL)
    {
        printf("\n[Baggage Queue Status]");
        printf("\nThe Baggage queue is currently empty.\n");
        return;
    }

    // Get the front luggage from the baggage queue
    Baggage *temp = baggageFront;
    printf("\n[Baggage Queue Status]");
    printf("\nPassenger Name: %s", temp->luggage->ownerName);

    // Update the front and rear pointers of the baggage queue
    if (baggageFront == baggageRear)
    {
        // Only one luggage in the queue
        baggageFront = baggageRear = NULL;
        printf("\nStatus: Baggage entered on the belt successfully. The queue is now empty.\n");
    }
    else
    {
        // Move to the next luggage in the queue
        baggageFront = baggageRear->next;
        printf("\nStatus: Baggage entered on the belt successfully. Waiting for the next baggage.\n");
    }

    // Push the luggage onto the baggage cart
    pushOnCart(temp->luggage);
    // Free the memory occupied by the dequeued luggage
    free(temp);
}

/**
 * Ends the baggage queue.
 */

/**
 * Begins the Baggage Cart.
 */

// Function to create a new baggage cart node
BaggageCart *createBaggageCart(Luggage *newLuggage)
{
    BaggageCart *baggageCart = (BaggageCart *)malloc(sizeof(BaggageCart));
    if (baggageCart == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Initialize the baggage cart with luggage and next pointer
    baggageCart->luggage = newLuggage;
    baggageCart->next = NULL;
    return baggageCart;
}

// Function to push luggage onto the baggage cart
void pushOnCart(Luggage *newLuggage)
{
    // Create a new baggage cart node
    BaggageCart *baggageCart = createBaggageCart(newLuggage);

    printf("\n+------------------------------------------------------+");
    printf("\n|                Baggage Cart Loading                  |");
    printf("\n+------------------------------------------------------+");

    if (baggageCart == NULL)
    {
        // Error handling for memory allocation failure
        printf("\n| Error: Unable to load the baggage onto the cart due   |");
        printf("\n|        to an internal error. Please try again.        |");
        printf("\n+------------------------------------------------------+");
    }
    else
    {
        // Update the stack pointer with the new baggage cart node
        baggageCart->next = stackHead;
        stackHead = baggageCart;

        // Display success message
        printf("\n| Success: Luggage has been loaded onto the cart.      |");
        printf("\n| Luggage Owner: %-37s |", newLuggage->ownerName);
        printf("\n+------------------------------------------------------+");
    }
    printf("\n");
}

// Function to pop luggage from the baggage cart
void popFromCart()
{
    printf("\n+------------------------------------------------------+");
    printf("\n|                Baggage Cart Unloading                |");
    printf("\n+------------------------------------------------------+");

    // Check if the baggage cart is empty
    if (stackHead == NULL)
    {
        printf("\n| Error: The cart is empty. No luggage to remove.       |");
        printf("\n+------------------------------------------------------+");
    }
    else
    {
        // Initialize temporary pointers for traversal
        BaggageCart *temp = stackHead;
        BaggageCart *prev = NULL;

        if (temp->next == NULL)
        {
            // Only one item in the stack
            printf("\n| Success: Luggage of %-31s has been loaded. |", temp->luggage->ownerName);
            stackHead = NULL;
            free(temp);
        }
        else
        {
            // More than one item in the stack
            while (temp->next != NULL)
            {
                prev = temp;
                temp = temp->next;
            }
            printf("\n| Success: Luggage of %-31s has been loaded. |", temp->luggage->ownerName);
            prev->next = NULL;
            free(temp);
        }
        printf("\n+------------------------------------------------------+");
    }
    printf("\n");
}

// Function to display the contents of the baggage cart stack
void displayStack()
{
    printf("\n+------------------------------------------------------+");
    printf("\n|                Baggage Cart Contents                 |");
    printf("\n+------------------------------------------------------+");

    // Check if the baggage cart is empty
    if (stackHead == NULL)
    {
        printf("\n| The cart is empty.                                   |");
    }
    else
    {
        // Traverse the baggage cart stack and display luggage details
        BaggageCart *temp = stackHead;
        printf("\n| %-20s | %-20s | %-6s |", "Luggage Owner", "Size", "Weight");
        printf("\n+------------------------------------------------------+");
        while (temp != NULL)
        {
            printf("\n| %-20s | %-20s | %-6.2f kg |", temp->luggage->ownerName, temp->luggage->size, temp->luggage->weight);
            printf("\n+------------------------------------------------------+");
            temp = temp->next;
        }
    }
    printf("\n");
}

/**
 * Ends the Baggage Cart.
 */

// Main function starts
int main()
{
    int choice;
    do
    {
        // Display menu for the passenger check-in system
        printf("\n+---------------------------------+");
        printf("\n| -- Passenger Check-In System -- |");
        printf("\n+---------------------------------+");
        printf("\n| 1. Register Passenger           |");
        printf("\n| 2. Process Check-In             |");
        printf("\n| 3. Display Passenger List       |");
        printf("\n| 4. Display Baggage Cart         |");
        printf("\n| 5. Unload Baggage Cart          |");
        printf("\n| 6. Terminate Program            |");
        printf("\n+---------------------------------+");
        printf("\nEnter your choice (1 to 6): ");

        // Get user choice
        scanf("%d", &choice);
        clearInputBuffer();

        // Switch case based on user choice
        switch (choice)
        {
        case 1:
            beginCheckInProcess();
            break;
        case 2:
            completePassengerCheckIn();

            break;
        case 3:
            viewPassengers();
            break;
        case 4:
            displayStack();
            break;
        case 5:
            if (stackHead == NULL)
            {
                printf("\nThe baggage cart is currently empty.\n");
            }
            else
            {
                while (stackHead != NULL)
                {
                    popFromCart();
                }
            }
            break;
        case 6:
            printf("\nThank you for using the Passenger Check-In System.");
            printf("\nHave a great day! Goodbye!\n\n");
            exit(0);
        default:
            printf("\nInvalid choice! Please select a valid option (1-6).\n");
            break;
        }
    } while (choice != 5);

    return 0;
}
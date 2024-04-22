import json
import msvcrt
import threading
import time
from datetime import datetime
from pprint import pprint

import matplotlib.pyplot as plt
import serial
from colorama import Fore, Style, init
from pynput import keyboard


def main():
    init(autoreset=True)
    
    porta_serial = None

    while porta_serial is None:
        # Ask for the COM port and baud rate
        com_port = input("Enter the COM port (e.g., COM1, COM2, etc.): ")
        baud_rate = int(input("Enter the baud rate: "))

        try:
            # Establish serial connection
            porta_serial = serial.Serial(com_port, baud_rate)
        except serial.SerialException:
            print(Fore.RED + f"Failed to establish connection with {com_port} at {baud_rate} baud rate. Please check the COM port and baud rate.")

    print(Fore.GREEN + "Connection established. Press 's' to exit.")
    time.sleep(1)

    # Start thread to check user input
    thread_input = threading.Thread(target=check_input, args=(porta_serial,))
    thread_input.daemon = True
    thread_input.start()

    buffer = ""
    store_data = False
    file_name = ""

    try:
        while True:
            
            # Check if the thread stopped
            if not thread_input.is_alive():
                porta_serial.close()
                break
            
            # Check if there is data available to read from the serial port
            elif porta_serial.in_waiting > 0:
                # Read data from the serial port
                data = porta_serial.readline().decode().strip()

                # Print the read data
                print(data)

                # Search within data for the "updown" and "downup" patterns
                # If the "updown" pattern is found, start storing the data
                if "updown" in data:
                    if store_data == False:
                        store_data = True
                        print(Fore.YELLOW + "Starting to store data...")
                        time.sleep(1)
                        buffer = ""

                # If the "downup" pattern is found, stop storing the data
                elif "downup" in data:
                    if store_data == True:
                        store_data = False
                        print(Fore.YELLOW + "Stopping storing data...")

                        time.sleep(1)
                        
                        # Generate file name based on the current time
                        file_name = datetime.now().strftime("%H_%M_%S")

                        if file_name:
                            # Write data to a .txt file
                            with open(f"{file_name}.txt", "w") as txt_file:
                                txt_file.write(buffer)

                            # Convert data to a JSON object and write to a .json file
                            data_json = {"data": buffer.split("\n")}
                            with open(f"{file_name}.json", "w") as json_file:
                                #json.dump(data_json, json_file)
                                pprint(data_json, json_file)
                        else:
                            print("File name not provided.")
                            
                        # Based on the data stored, plot the graph
                        print("Showing plot...")
                        # Now plot the data 
                        with open(f"{file_name}.txt", "r") as file:
                            data = file.readlines()
                            x = []
                            y = []
                            for line in data:
                                line = line.strip().split(";")
                                x.append(line[0])
                                print(line[0])
                                print(line[1])
                                y.append(int(line[1]))

                            fig, ax = plt.subplots(1, figsize=(12,6))
                            
                            sc = ax.plot(x, y)
                            plt.xlabel("Time")
                            plt.ylabel("Temperature")
                            plt.title("Temperature vs Time")
                            plt.xticks(rotation=45)
                            
                            # Show only the initial x label value and the last one
                            plt.gca().set_xticks(plt.gca().get_xticks()[::len(plt.gca().get_xticks())-1])
                            
                            plt.grid(True)

                            plt.show()
                            
                    
                # If storing data, add it to the buffer
                elif store_data:
                    # Check for the string "Temperature" in the data
                    if "Temperature: " in data:
                        # Remove all the characters before the string "Temperature", including it
                        data = data[data.find("Temperature: ") + len("Temperature: "):]
                        
                        # Read the next 3 digits after the string "Temperature"
                        data = data[:3]
                        
                        # Get the current time
                        current_time = datetime.now().strftime("%H:%M:%S")
                        
                        buffer += current_time + ";" + data + "\n"
                

            # Wait a short period to avoid overloading the serial port
            time.sleep(0.01)
    except KeyboardInterrupt:
        print("\nProgram terminated by user.")
    finally:
        # Close the serial port before exiting
        if porta_serial is not None and porta_serial.is_open:
            porta_serial.close()

def check_input(porta_serial):
    """
    Function to check user input.
    """
    while True:
        # Read user input
        user_input = msvcrt.getch()
        
        # Format the input to be a string
        user_input = user_input.decode()

        if user_input == "s":
            print(Fore.BLUE + "Exiting program...")
            break
        else:
            # Send the input via the serial port
            porta_serial.write(user_input.encode())
            
    
if __name__ == "__main__":
    main()
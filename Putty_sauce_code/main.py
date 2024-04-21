import serial
import threading
import time
import json
from datetime import datetime

def main():
    porta_serial = None

    while porta_serial is None:
        # Ask for the COM port and baud rate
        com_port = input("Enter the COM port (e.g., COM1, COM2, etc.): ")
        baud_rate = int(input("Enter the baud rate: "))

        try:
            # Establish serial connection
            porta_serial = serial.Serial(com_port, baud_rate)
        except serial.SerialException:
            print(f"Failed to establish connection with {com_port} at {baud_rate} baud rate. Please check the COM port and baud rate.")

    print("Connection established. Press Ctrl+C to exit.")

    # Start thread to check user input
    thread_input = threading.Thread(target=check_input, args=(porta_serial,))
    thread_input.daemon = True
    thread_input.start()

    buffer = ""
    store_data = False
    file_name = ""

    try:
        while True:
            # Check if there is data available to read from the serial port
            if porta_serial.in_waiting > 0:
                # Read data from the serial port
                data = porta_serial.readline().decode().strip()

                # Print the read data
                print(data)

                # If the "updown" pattern is found, start storing the data
                if data == "updown":
                    store_data = True
                    print("Starting to store data...")
                    buffer = ""

                # If the "downup" pattern is found, stop storing the data
                elif data == "downup":
                    store_data = False
                    print("Stopping storing data...")
                    print("Stored data:", buffer)

                    # Generate file name based on the current time
                    file_name = datetime.now().strftime("%H_%M_%S")

                    if file_name:
                        # Write data to a .txt file
                        with open(f"{file_name}.txt", "w") as txt_file:
                            txt_file.write(buffer)

                        # Convert data to a JSON object and write to a .json file
                        data_json = {"data": buffer.split("\n")}
                        with open(f"{file_name}.json", "w") as json_file:
                            json.dump(data_json, json_file)
                    else:
                        print("File name not provided.")

                    file_name = ""

                # If storing data, add it to the buffer
                elif store_data:
                    buffer += data + "\n"

            # Wait a short period to avoid overloading the serial port
            time.sleep(0.1)
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
        user_input = input()

        # If the input is "downup", send the pattern via the serial port
        if user_input == "downup":
            porta_serial.write(user_input.encode())
        else:
            # Send the input via the serial port
            porta_serial.write(user_input.encode())

if __name__ == "__main__":
    main()
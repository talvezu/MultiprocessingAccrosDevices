from flask import Flask, Response, request, stream_with_context
import numpy as np
import time
import json

app = Flask(__name__)
vector_length = 50


@app.route('/enable_stream', methods=['POST'])
def enable_stream():
    client_id = request.args.get('client_id')

    if client_id:
        # Create a response generator for the client
        def generate():
            yield from generate_data()  # Call the generate_data function
        response = Response(
            stream_with_context(
                generate()),
            mimetype='text/plain')

        return response

    return 'Invalid request'


def generate_data():
    """Generator function to simulate streaming data"""
    while True:
        print("yield")
        yield str(time.time()) + '\n'  # Generate a timestamp as the data
        time.sleep(1)


@app.route('/register_for_vector_stream', methods=['POST'])
def register_for_vector_generating_service():
    client_id = request.form.get('client_id')
    frequency = request.form.get('frequency')
    sleep_time = 10 / float(frequency)  # 10**9

    if client_id:
        # Create a response generator for the client
        def generate_vector():
            """Generator function to simulate streaming data"""
            response_data = {
                'status': 'success',
                'message': 'Registered for vector stream'
            }
            response_json = json.dumps(response_data)

            yield response_json.encode('utf-8')  # Yield the JSON response

            while True:
                vector = np.random.normal(size=vector_length)
                vector_list = vector.tolist()  # Convert NumPy array to Python list
                vector_json = json.dumps(vector_list)
                # Yield the vector as JSON serialized string
                yield vector_json.encode('utf-8')

        response = Response(
            stream_with_context(generate_vector()),
            mimetype='application/octet-stream')  # Set the response mimetype as raw binary

        time.sleep(sleep_time)
        return response

    return 'Invalid request'


def generate_vector(sleep_time):
    """Generator function to simulate streaming data"""
    while True:
        print("yield")
        # yield np.random.normal(size=vector_length)  # generate random vector
        vec = np.random.normal(size=vector_length)  # generate random vector
        print(vec)
        yield vec
        time.sleep(sleep_time)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)

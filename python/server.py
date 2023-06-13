from flask import Flask, Response, request, stream_with_context
import time

app = Flask(__name__)


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


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)

# Use the official Python base image
FROM python:3.9

# Set the build argument for the root directory path
ARG PROJECT_ROOT

# Install required packages
RUN which python
RUN /usr/local/bin/python --version
RUN which pip
RUN /usr/local/bin/pip --version
#RUN /usr/local/bin/python --version

# Upgrade pip
RUN pip install --upgrade pip

# Install required packages
# -m pip, command is executed within the context of the specified Python version
RUN apt-get update && apt-get install -y cmake
RUN python3 -m pip install flask

# Sanity test for installed component
RUN python -c "from flask import Flask"

# Set the working directory in the container
WORKDIR /

ADD python /python
ADD requirements.txt /

# Sanify to show the requirements file copied
RUN ["find", "/", "-name", "requirements.txt"]

# Print the working directory
RUN pwd

# Install project dependencies
RUN pip install --no-cache-dir -r /requirements.txt

# Expose the desired port (e.g., 8000)
EXPOSE 8000

# Start the server
CMD ["python", "./python/server.py"]

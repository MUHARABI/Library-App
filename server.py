from flask import Flask, request, jsonify, send_from_directory
import subprocess
import os

app = Flask(__name__)

@app.route('/execute', methods=['POST'])
def execute_cpp():
    command = request.json.get('command')
    result = subprocess.run(['./library.exe', command], capture_output=True, text=True)
    return jsonify({'output': result.stdout})

@app.route('/')
def serve_index():
    return send_from_directory('.', 'index.html')

@app.route('/<path:filename>')
def serve_static(filename):
    return send_from_directory('static', filename)

if __name__ == '__main__':
    app.run(debug=True)

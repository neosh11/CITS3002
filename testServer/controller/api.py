import json

def test(obj):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', 'application/json')
    obj.end_headers()

    m = {'id': 1, 'name': 'Neosh'}
    n = json.dumps(m)
    
    obj.wfile.write(bytes(n, 'utf-8'))
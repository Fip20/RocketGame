from flask import Flask, jsonify

app = Flask(__name__)

# Ranking memory
classifica = [
    {"position": "1", "score": 289},
    {"position": "2", "score": 230},
    {"position": "3", "score": 166}
]

@app.route('/get_ranking', methods=['GET'])
def get_classifica():
    # Ranking from top to down
    sorted_classifica = sorted(classifica, key=lambda x: x['score'], reverse=True)
    return jsonify(sorted_classifica)

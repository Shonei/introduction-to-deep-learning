import React, { Component } from 'react';
import {Row, Col} from 'react-materialize';
import scrabble from '../img/scrabble.jpg';
import Item from './Item';

class Grid extends Component {
  render() {
    return (
      <div className="container">
        <Row>
          <Col s={12} m={4}>
            <Item title="Scrabble" URL="scrabble" img={scrabble} />
          </Col>
        </Row>
      </div>
    );
  }
}

export default Grid;
import React from 'react';
import EStyleSheet from 'react-native-extended-stylesheet';

import Home from './screens/Home';

EStyleSheet.build({
  $backgroundGray: '#ddd',
});

export default () => <Home />;
import numpy

import time
import theano
import theano.tensor as T

n_in = 784  # 28 * 28
n_hidden = 500
n_out = 10

def as_f32(v):
    return numpy.asarray(v).astype('float32')

class Model(object):
    def __init__(self, params):
        # This should respect the spec in params rather than use the
        # fixed arch below.
        print("In init.")
        print(str(self))

        self.W1 = theano.shared(value=numpy.asarray(
                numpy.random.uniform(
                    low=-numpy.sqrt(6. / (n_in + n_hidden)),
                    high=numpy.sqrt(6. / (n_in + n_hidden)),
                    size=(n_in, n_hidden)),
                dtype='float32'),
                name='W1', borrow=True)

        self.b1 = theano.shared(value=numpy.zeros((n_hidden,), dtype='float32'),
                                name='b1', borrow=True)

        self.W2 = theano.shared(value=numpy.zeros((n_hidden, n_out), dtype='float32'),
                                name='W2', borrow=True)
        self.b2 = theano.shared(value=numpy.zeros((n_out,), dtype='float32'),
                                name='b2', borrow=True)
        self.lr = theano.shared(value=numpy.asarray(0.1, dtype='float32'),
                                name='lr', borrow=True)

        self.params = (self.W1, self.b1, self.W2, self.b2)
        self.grads = [theano.shared(numpy.zeros_like(param.get_value(borrow=True)))
                      for param in self.params]

        x = T.fmatrix('x')
        y = T.fmatrix('y')

        # Drop the last dim (which should be 1)
        #y_r = y.dimshuffle(0)

        hidden = T.tanh(T.dot(x, self.W1) + self.b1)
        p_y_given_x = T.nnet.softmax(T.dot(hidden, self.W2) + self.b2)
        pred = T.argmax(p_y_given_x, axis=1)
        nll = -T.mean(T.log(p_y_given_x)[T.arange(y.shape[0]), T.arange(y.shape[0])])

        L2_sqr = (self.W1 ** 2).sum() + (self.W2 ** 2).sum()

        cost = nll + L2_sqr * as_f32(0.0001)

        gparams = [T.grad(cost, param) for param in self.params]

        updates = [(grad, grad - self.lr * gparam)
                   for grad, gparam in zip(self.grads, gparams)]

        # This does not update values, only accumulate gradients
        self.train = theano.function([x, y], cost, updates=updates)

        self.test = theano.function([x, y], cost)

    def size(self):
        return self.W1.size + self.W2.size + self.b1.size + self.b2.size

    @staticmethod
    def updbuf(buf, val, p, acc=False):
        l = val.size
        #import pdb; pdb.set_trace()
        if acc:
            buf[p:p+l] += val.flatten()
        else:
            buf[p:p+l] = val.flatten()
        return p+l

    def get_grads(self, buf):
        s = 0
        for g in self.grads:
            s = self.updbuf(buf, g.get_value(borrow=True), s)

    def acc_grads(self, buf):
        s = 0
        for g in self.grads:
            s = self.updbuf(buf, g.get_value(borrow=True), s, acc=True)

    def upd_lr(self, newLR):
        self.lr.set_value(newLR)

    def get_params(self, buf):
        print(self)
        s = 0
        tot_size = 0
        for p in self.params:
            val = p.get_value(borrow=True)
            tot_size += val.size
            if buf.size == 0:
                buf = numpy.zeros(tot_size, dtype=numpy.float32)
            elif buf.size < tot_size:
                buf.resize(tot_size)

            s = self.updbuf(buf, val, s)

    def set_params(self, buf):
        print(self)
        s = 0
        for p in self.params:
            l = p.get_value(borrow=True).size
            #import pdb; pdb.set_trace()
            p.set_value(T.reshape(buf[s:s+l], p.shape))
            s += l

    # This doesn't have adagrad yet, it's just to make sure the rest works.
    def upd_params(self, buf, numMB):
        s = 0
        for p in self.params:
            pv = p.get_value(borrow=True)
            l = pv.size
            p.set_value(pv + T.reshape(buf[s:s+l], pv.shape))
            s += l


def myinit(params):
    print("Golden: In init with params ")
    print(str(params))
    return Model(params)

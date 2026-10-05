#!/usr/bin/env python

import torch
import torch.nn as nn
import torch.optim as optim
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import numpy as np
import pathlib

transform = transforms.Compose([
  transforms.ToTensor(),
  transforms.Normalize((0.1307,), (0.3081,)),
])

train_dataset = torchvision.datasets.MNIST(
  root='./mnist',
  train=True,
  download=True,
  transform=transform,
)

test_dataset = torchvision.datasets.MNIST(
  root='./mnist',
  train=False,
  download=True,
  transform=transform,
)

train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)

class DigitNN(nn.Module):
  def __init__(self):
    super(DigitNN, self).__init__()
    self.flatten = nn.Flatten()
    self.relu = nn.ReLU()
    self.fc1 = nn.Linear(28 * 28, 128)
    self.fc2 = nn.Linear(128, 64)
    self.fc3 = nn.Linear(64, 10)

  def forward(self, x):
    x = self.flatten(x)
    x = self.fc1(x)
    x = self.relu(x)
    x = self.fc2(x)
    x = self.relu(x)
    x = self.fc3(x)
    return x

model = DigitNN()

criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9)

device = torch.device("cpu")

num_epochs = 5
train_losses = []

for epoch in range(num_epochs):
  running_loss = 0.0

  for i, (inputs, labels) in enumerate(train_loader):
    inputs, labels = inputs.to(device), labels.to(device)
    optimizer.zero_grad()
    outputs = model(inputs)
    loss = criterion(outputs, labels)
    loss.backward()
    optimizer.step()
    running_loss += loss.item()
    if (i + 1) % 100 == 0:
      print(f"epoch: {epoch + 1}/{num_epochs}, step: {i + 1}/{len(train_loader)}, loss: {running_loss / 100.0}")
      train_losses.append(running_loss / 100.0)
      running_loss = 0.0

torch.save(model.state_dict(), 'digit_model.pth')
pathlib.Path("./model").mkdir(exist_ok=True)

layers = [ model.fc1, model.fc2, model.fc3 ]
for i, layer in enumerate(layers):
  weights = layer.weight.detach().cpu().numpy().astype(np.float32).T
  biases = layer.bias.detach().cpu().numpy().astype(np.float32)

  np.ascontiguousarray(weights).tofile(f"model/layer{i + 1}_weights.bin")
  biases.tofile(f"model/layer{i + 1}_biases.bin")

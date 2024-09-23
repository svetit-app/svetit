import { Component, inject } from '@angular/core';

import { MessageService } from '../message.service';

@Component({
    selector: 'app-messages',
    templateUrl: './messages.component.html',
    styleUrls: ['./messages.component.css'],
    standalone: true
})
export class MessagesComponent {
  messageService = inject(MessageService);
}

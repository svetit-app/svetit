import { Component, OnInit, inject } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { HttpClient } from '@angular/common/http';

import { catchError, tap } from 'rxjs/operators';
import { pipe } from 'rxjs';

@Component({
    selector: 'app-tg-auth',
    templateUrl: './tg-auth.component.html',
    styleUrls: ['./tg-auth.component.css'],
    standalone: true
})
export class TgAuthComponent implements OnInit {
  private route = inject(ActivatedRoute);
  private http = inject(HttpClient);


  text: string;

  ngOnInit() {
    const p_str: string = 'token';
    if (this.route.snapshot.paramMap.has(p_str))
    {
      let token = this.route.snapshot.paramMap.get(p_str);
      console.log(token);
      if (token.length)
      {
        this.text = "Идёт привязка Telegram пользователя. Пожалуйста ждите...";
        const body = {token};
        this.http.post('/api/v2/bot/auth/', body).pipe(
          tap(() => this.text = "Telegram пользователь успешно привязан"),
          catchError((error: any) => this.text = "Не удалось привязать Telegram пользователя: " + error.message)
        ).subscribe();
        return;
      }
    }

    this.text = "Неверный параметр";
    
    console.log(this.route.parent.snapshot.paramMap.get(p_str));
  }
}

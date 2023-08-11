import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { TgAuthComponent } from './tg-auth.component';

describe('TgAuthComponent', () => {
  let component: TgAuthComponent;
  let fixture: ComponentFixture<TgAuthComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ TgAuthComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TgAuthComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
